// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_DATA_STORE_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_DATA_STORE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/important_file_writer.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/app_list/search/common/dictionary_data_store.h"
#include "chrome/browser/ui/app_list/search/history_data.h"

namespace base {
class DictionaryValue;
class SequencedTaskRunner;
}

namespace app_list {

namespace test {
class HistoryDataStoreTest;
}

class HistoryDataStore : public base::RefCountedThreadSafe<HistoryDataStore> {
 public:
  typedef base::Callback<void(scoped_ptr<HistoryData::Associations>)>
      OnLoadedCallback;

  explicit HistoryDataStore(const base::FilePath& data_file);

  
  
  void Flush(const DictionaryDataStore::OnFlushedCallback& on_flushed);

  
  
  
  void Load(const OnLoadedCallback& on_loaded);

  
  void SetPrimary(const std::string& query, const std::string& result);
  void SetSecondary(const std::string& query,
                    const HistoryData::SecondaryDeque& results);
  void SetUpdateTime(const std::string& query, const base::Time& update_time);
  void Delete(const std::string& query);

 private:
  friend class base::RefCountedThreadSafe<HistoryDataStore>;
  friend class app_list::test::HistoryDataStoreTest;

  virtual ~HistoryDataStore();

  
  base::DictionaryValue* GetAssociationDict();

  
  base::DictionaryValue* GetEntryDict(const std::string& query);

  void OnDictionaryLoadedCallback(OnLoadedCallback callback,
                                  scoped_ptr<base::DictionaryValue> dict);

  scoped_refptr<DictionaryDataStore> data_store_;

  DISALLOW_COPY_AND_ASSIGN(HistoryDataStore);
};

}  

#endif  
