// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_INTERCEPTOR_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_INTERCEPTOR_H_

#include "base/memory/singleton.h"
#include "net/url_request/url_request.h"
#include "url/gurl.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/resource_type.h"

namespace appcache {

class AppCacheRequestHandler;
class AppCacheService;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheInterceptor
    : public net::URLRequest::Interceptor {
 public:
  
  
  static void EnsureRegistered() {
    CHECK(GetInstance());
  }

  
  static void SetExtraRequestInfo(net::URLRequest* request,
                                  AppCacheService* service,
                                  int process_id,
                                  int host_id,
                                  ResourceType::Type resource_type);

  
  
  static void GetExtraResponseInfo(net::URLRequest* request,
                                   int64* cache_id,
                                   GURL* manifest_url);

  
  static void PrepareForCrossSiteTransfer(net::URLRequest* request,
                                          int old_process_id);
  static void CompleteCrossSiteTransfer(net::URLRequest* request,
                                        int new_process_id,
                                        int new_host_id);

  static AppCacheInterceptor* GetInstance();

 protected:
  
  virtual net::URLRequestJob* MaybeIntercept(
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate) OVERRIDE;
  virtual net::URLRequestJob* MaybeInterceptResponse(
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate) OVERRIDE;
  virtual net::URLRequestJob* MaybeInterceptRedirect(
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate,
      const GURL& location) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<AppCacheInterceptor>;

  AppCacheInterceptor();
  virtual ~AppCacheInterceptor();

  static void SetHandler(net::URLRequest* request,
                         AppCacheRequestHandler* handler);
  static AppCacheRequestHandler* GetHandler(net::URLRequest* request);

  DISALLOW_COPY_AND_ASSIGN(AppCacheInterceptor);
};

}  

#endif  
