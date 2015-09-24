// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_APPCACHE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_APPCACHE_HELPER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "content/browser/appcache/chrome_appcache_service.h"
#include "googleurl/src/gurl.h"
#include "net/url_request/url_request_context_getter.h"

class Profile;

class BrowsingDataAppCacheHelper
    : public base::RefCountedThreadSafe<BrowsingDataAppCacheHelper> {
 public:
  explicit BrowsingDataAppCacheHelper(Profile* profile);

  virtual void StartFetching(Callback0::Type* completion_callback);
  virtual void CancelNotification();
  virtual void DeleteAppCacheGroup(const GURL& manifest_url);

  appcache::AppCacheInfoCollection* info_collection() const {
    DCHECK(!is_fetching_);
    return info_collection_;
  }

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataAppCacheHelper>;
  virtual ~BrowsingDataAppCacheHelper();

  scoped_ptr<Callback0::Type> completion_callback_;
  scoped_refptr<appcache::AppCacheInfoCollection> info_collection_;

 private:
  void OnFetchComplete(int rv);
  ChromeAppCacheService* GetAppCacheService();

  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;
  bool is_fetching_;
  scoped_refptr<net::CancelableCompletionCallback<BrowsingDataAppCacheHelper> >
      appcache_info_callback_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataAppCacheHelper);
};

class CannedBrowsingDataAppCacheHelper : public BrowsingDataAppCacheHelper {
 public:
  explicit CannedBrowsingDataAppCacheHelper(Profile* profile);

  
  
  
  CannedBrowsingDataAppCacheHelper* Clone();

  
  
  void AddAppCache(const GURL& manifest_url);

  
  void Reset();

  
  bool empty() const;

  
  virtual void StartFetching(Callback0::Type* completion_callback);
  virtual void CancelNotification() {}

 private:
  virtual ~CannedBrowsingDataAppCacheHelper();

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataAppCacheHelper);
};

#endif  
