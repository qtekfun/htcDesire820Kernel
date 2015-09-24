// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WORKER_HOST_WORKER_STORAGE_PARTITION_H_
#define CONTENT_BROWSER_WORKER_HOST_WORKER_STORAGE_PARTITION_H_

#include "base/memory/ref_counted.h"

namespace quota {
class QuotaManager;
}

namespace fileapi {
class FileSystemContext;
}  

namespace net {
class URLRequestContextGetter;
}

namespace webkit_database {
class DatabaseTracker;
}  

namespace content {
class ChromeAppCacheService;
class IndexedDBContextImpl;

class WorkerStoragePartition {
 public:
  WorkerStoragePartition(
      net::URLRequestContextGetter* url_request_context,
      net::URLRequestContextGetter* media_url_request_context,
      ChromeAppCacheService* appcache_service,
      quota::QuotaManager* quota_manager,
      fileapi::FileSystemContext* filesystem_context,
      webkit_database::DatabaseTracker* database_tracker,
      IndexedDBContextImpl* indexed_db_context);
  ~WorkerStoragePartition();

  
  
  
  WorkerStoragePartition(const WorkerStoragePartition& other);
  const WorkerStoragePartition& operator=(const WorkerStoragePartition& rhs);

  bool Equals(const WorkerStoragePartition& other) const;

  net::URLRequestContextGetter* url_request_context() const {
    return url_request_context_.get();
  }

  net::URLRequestContextGetter* media_url_request_context() const {
    return media_url_request_context_.get();
  }

  ChromeAppCacheService* appcache_service() const {
    return appcache_service_.get();
  }

  quota::QuotaManager* quota_manager() const {
    return quota_manager_.get();
  }

  fileapi::FileSystemContext* filesystem_context() const {
    return filesystem_context_.get();
  }

  webkit_database::DatabaseTracker* database_tracker() const {
    return database_tracker_.get();
  }

  IndexedDBContextImpl* indexed_db_context() const {
    return indexed_db_context_.get();
  }

 private:
  void Copy(const WorkerStoragePartition& other);

  scoped_refptr<net::URLRequestContextGetter> url_request_context_;
  scoped_refptr<net::URLRequestContextGetter> media_url_request_context_;
  scoped_refptr<ChromeAppCacheService> appcache_service_;
  scoped_refptr<quota::QuotaManager> quota_manager_;
  scoped_refptr<fileapi::FileSystemContext> filesystem_context_;
  scoped_refptr<webkit_database::DatabaseTracker> database_tracker_;
  scoped_refptr<IndexedDBContextImpl> indexed_db_context_;
};

}  

#endif  
