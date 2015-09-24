// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_QUOTA_QUOTA_CLIENT_H_
#define WEBKIT_BROWSER_QUOTA_QUOTA_CLIENT_H_

#include <list>
#include <set>
#include <string>

#include "base/callback.h"
#include "url/gurl.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/quota/quota_types.h"

namespace quota {

class WEBKIT_STORAGE_BROWSER_EXPORT QuotaClient {
 public:
  typedef base::Callback<void(int64 usage)> GetUsageCallback;
  typedef base::Callback<void(const std::set<GURL>& origins)>
      GetOriginsCallback;
  typedef base::Callback<void(QuotaStatusCode status)> DeletionCallback;

  virtual ~QuotaClient() {}

  enum ID {
    kUnknown = 1 << 0,
    kFileSystem = 1 << 1,
    kDatabase = 1 << 2,
    kAppcache = 1 << 3,
    kIndexedDatabase = 1 << 4,
    kAllClientsMask = -1,
  };

  virtual ID id() const = 0;

  
  virtual void OnQuotaManagerDestroyed() = 0;

  
  
  
  
  virtual void GetOriginUsage(const GURL& origin_url,
                              StorageType type,
                              const GetUsageCallback& callback) = 0;

  
  
  
  virtual void GetOriginsForType(StorageType type,
                                 const GetOriginsCallback& callback) = 0;

  
  
  
  virtual void GetOriginsForHost(StorageType type,
                                 const std::string& host,
                                 const GetOriginsCallback& callback) = 0;

  
  
  virtual void DeleteOriginData(const GURL& origin,
                                StorageType type,
                                const DeletionCallback& callback) = 0;

  virtual bool DoesSupport(StorageType type) const = 0;
};

typedef std::list<QuotaClient*> QuotaClientList;

}  

#endif  
