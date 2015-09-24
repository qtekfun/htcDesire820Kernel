// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEBUI_URL_DATA_SOURCE_IMPL_H_
#define CONTENT_BROWSER_WEBUI_URL_DATA_SOURCE_IMPL_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/browser/webui/url_data_manager.h"
#include "content/common/content_export.h"

namespace base {
class RefCountedMemory;
}

namespace content {
class URLDataManagerBackend;
class URLDataSource;
class URLDataSourceImpl;

struct DeleteURLDataSource {
  static void Destruct(const URLDataSourceImpl* data_source) {
    URLDataManager::DeleteDataSource(data_source);
  }
};

class URLDataSourceImpl : public base::RefCountedThreadSafe<
    URLDataSourceImpl, DeleteURLDataSource> {
 public:
  
  
  URLDataSourceImpl(const std::string& source_name,
                    URLDataSource* source);

  
  
  
  virtual void SendResponse(int request_id, base::RefCountedMemory* bytes);

  const std::string& source_name() const { return source_name_; }
  URLDataSource* source() const { return source_.get(); }

 protected:
  virtual ~URLDataSourceImpl();

 private:
  friend class URLDataManager;
  friend class URLDataManagerBackend;
  friend class base::DeleteHelper<URLDataSourceImpl>;

  
  
  virtual void SendResponseOnIOThread(
      int request_id,
      scoped_refptr<base::RefCountedMemory> bytes);

  
  
  
  const std::string source_name_;

  
  
  
  
  
  
  
  URLDataManagerBackend* backend_;

  scoped_ptr<URLDataSource> source_;
};

}  

#endif  
