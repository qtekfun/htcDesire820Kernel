// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_STORAGE_PARTITION_H_
#define CONTENT_PUBLIC_BROWSER_STORAGE_PARTITION_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"

class GURL;

namespace appcache {
class AppCacheService;
}

namespace fileapi {
class FileSystemContext;
}

namespace net {
class URLRequestContextGetter;
}

namespace quota {
class QuotaManager;
class SpecialStoragePolicy;
}

namespace webkit_database {
class DatabaseTracker;
}

namespace content {

class BrowserContext;
class IndexedDBContext;
class DOMStorageContext;

class StoragePartition {
 public:
  virtual base::FilePath GetPath() = 0;
  virtual net::URLRequestContextGetter* GetURLRequestContext() = 0;
  virtual net::URLRequestContextGetter* GetMediaURLRequestContext() = 0;
  virtual quota::QuotaManager* GetQuotaManager() = 0;
  virtual appcache::AppCacheService* GetAppCacheService() = 0;
  virtual fileapi::FileSystemContext* GetFileSystemContext() = 0;
  virtual webkit_database::DatabaseTracker* GetDatabaseTracker() = 0;
  virtual DOMStorageContext* GetDOMStorageContext() = 0;
  virtual IndexedDBContext* GetIndexedDBContext() = 0;

  enum RemoveDataMask {
    REMOVE_DATA_MASK_APPCACHE = 1 << 0,
    REMOVE_DATA_MASK_COOKIES = 1 << 1,
    REMOVE_DATA_MASK_FILE_SYSTEMS = 1 << 2,
    REMOVE_DATA_MASK_INDEXEDDB = 1 << 3,
    REMOVE_DATA_MASK_LOCAL_STORAGE = 1 << 4,
    REMOVE_DATA_MASK_SHADER_CACHE = 1 << 5,
    REMOVE_DATA_MASK_WEBSQL = 1 << 6,
    REMOVE_DATA_MASK_WEBRTC_IDENTITY = 1 << 7,
    REMOVE_DATA_MASK_ALL = -1
  };

  enum QuotaManagedStorageMask {
    
    QUOTA_MANAGED_STORAGE_MASK_TEMPORARY = 1 << 0,

    
    QUOTA_MANAGED_STORAGE_MASK_PERSISTENT = 1 << 1,

    
    QUOTA_MANAGED_STORAGE_MASK_SYNCABLE = 1 << 2,

    QUOTA_MANAGED_STORAGE_MASK_ALL = -1
  };

  
  
  
  
  
  
  
  
  
  
  
  virtual void ClearDataForOrigin(uint32 remove_mask,
                                  uint32 quota_storage_remove_mask,
                                  const GURL& storage_origin,
                                  net::URLRequestContextGetter* rq_context) = 0;

  
  
  
  typedef base::Callback<bool(const GURL&,
                              quota::SpecialStoragePolicy*)>
      OriginMatcherFunction;

  
  
  
  
  
  
  virtual void ClearData(uint32 remove_mask,
                         uint32 quota_storage_remove_mask,
                         const GURL* storage_origin,
                         const OriginMatcherFunction& origin_matcher,
                         const base::Time begin,
                         const base::Time end,
                         const base::Closure& callback) = 0;

 protected:
  virtual ~StoragePartition() {}
};

}  

#endif  
