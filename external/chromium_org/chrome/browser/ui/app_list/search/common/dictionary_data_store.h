// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_DICTIONARY_DATA_STORE_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_DICTIONARY_DATA_STORE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/important_file_writer.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class SequencedTaskRunner;
}

namespace app_list {

class DictionaryDataStore
    : public base::RefCountedThreadSafe<DictionaryDataStore>,
      public base::ImportantFileWriter::DataSerializer {
 public:
  typedef base::Callback<void(
      scoped_ptr<base::DictionaryValue>)> OnLoadedCallback;
  typedef base::Closure OnFlushedCallback;

  explicit DictionaryDataStore(const base::FilePath& data_file);

  
  void Flush(const OnFlushedCallback& on_flushed);

  
  
  
  void Load(const OnLoadedCallback& on_loaded);

  
  void ScheduleWrite();

  
  
  base::DictionaryValue* cached_dict() { return cached_dict_.get(); }

 private:
  friend class base::RefCountedThreadSafe<DictionaryDataStore>;

  virtual ~DictionaryDataStore();

  
  scoped_ptr<base::DictionaryValue> LoadOnBlockingPool();

  
  virtual bool SerializeData(std::string* data) OVERRIDE;

  base::FilePath data_file_;
  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  scoped_ptr<base::ImportantFileWriter> writer_;

  
  scoped_ptr<base::DictionaryValue> cached_dict_;

  DISALLOW_COPY_AND_ASSIGN(DictionaryDataStore);
};

}  

#endif  
