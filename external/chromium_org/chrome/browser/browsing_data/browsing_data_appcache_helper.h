// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_APPCACHE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_APPCACHE_HELPER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "net/base/completion_callback.h"
#include "url/gurl.h"
#include "webkit/browser/appcache/appcache_service.h"

class Profile;

namespace appcache {
class AppCacheService;
}

class BrowsingDataAppCacheHelper
    : public base::RefCountedThreadSafe<BrowsingDataAppCacheHelper> {
 public:
  typedef std::map<GURL, appcache::AppCacheInfoVector> OriginAppCacheInfoMap;

  explicit BrowsingDataAppCacheHelper(Profile* profile);

  virtual void StartFetching(const base::Closure& completion_callback);
  virtual void DeleteAppCacheGroup(const GURL& manifest_url);

  appcache::AppCacheInfoCollection* info_collection() const {
    DCHECK(!is_fetching_);
    return info_collection_.get();
  }

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataAppCacheHelper>;
  virtual ~BrowsingDataAppCacheHelper();

  base::Closure completion_callback_;
  scoped_refptr<appcache::AppCacheInfoCollection> info_collection_;

 private:
  void OnFetchComplete(int rv);

  bool is_fetching_;
  appcache::AppCacheService* appcache_service_;
  net::CancelableCompletionCallback appcache_info_callback_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataAppCacheHelper);
};

class CannedBrowsingDataAppCacheHelper : public BrowsingDataAppCacheHelper {
 public:
  explicit CannedBrowsingDataAppCacheHelper(Profile* profile);

  
  
  
  CannedBrowsingDataAppCacheHelper* Clone();

  
  
  void AddAppCache(const GURL& manifest_url);

  
  void Reset();

  
  bool empty() const;

  
  size_t GetAppCacheCount() const;

  
  const OriginAppCacheInfoMap& GetOriginAppCacheInfoMap() const;

  
  virtual void StartFetching(const base::Closure& completion_callback) OVERRIDE;

 private:
  virtual ~CannedBrowsingDataAppCacheHelper();

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataAppCacheHelper);
};

#endif  
