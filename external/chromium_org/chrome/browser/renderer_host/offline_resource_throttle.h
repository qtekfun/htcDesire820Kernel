// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_OFFLINE_RESOURCE_THROTTLE_H_
#define CHROME_BROWSER_RENDERER_HOST_OFFLINE_RESOURCE_THROTTLE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/offline/offline_load_page.h"
#include "content/public/browser/resource_throttle.h"
#include "net/base/completion_callback.h"

namespace appcache {
class AppCacheService;
}

namespace net {
class URLRequest;
}
class OfflineResourceThrottle
    : public content::ResourceThrottle,
      public base::SupportsWeakPtr<OfflineResourceThrottle> {
 public:
  OfflineResourceThrottle(net::URLRequest* request,
                          appcache::AppCacheService* appcache_service);
  virtual ~OfflineResourceThrottle();

  
  virtual void WillStartRequest(bool* defer) OVERRIDE;
  virtual const char* GetNameForLogging() const OVERRIDE;

 private:
  
  void OnBlockingPageComplete(bool proceed);

  
  void ClearRequestInfo();
  bool IsRemote(const GURL& url) const;

  
  bool ShouldShowOfflinePage(const GURL& url) const;

  
  void OnCanHandleOfflineComplete(int rv);

  net::URLRequest* request_;
  
  appcache::AppCacheService* appcache_service_;
  net::CancelableCompletionCallback appcache_completion_callback_;

  DISALLOW_COPY_AND_ASSIGN(OfflineResourceThrottle);
};

#endif  
