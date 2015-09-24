// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_SAFE_BROWSING_RESOURCE_THROTTLE_FACTORY_H_
#define CHROME_BROWSER_RENDERER_HOST_SAFE_BROWSING_RESOURCE_THROTTLE_FACTORY_H_

#include "base/basictypes.h"

class SafeBrowsingService;

namespace content {
class ResourceThrottle;
}

namespace net {
class URLRequest;
}

class SafeBrowsingResourceThrottleFactory {
 public:
#if defined(MOBILE_SAFE_BROWSING)
  
  
  static void RegisterFactory(SafeBrowsingResourceThrottleFactory* factory) {
    factory_ = factory;
  }
#endif

  
  static content::ResourceThrottle* Create(
      net::URLRequest* request,
      bool is_subresource,
      SafeBrowsingService* service);

 protected:
  SafeBrowsingResourceThrottleFactory() { }
  virtual ~SafeBrowsingResourceThrottleFactory() { }

  virtual content::ResourceThrottle* CreateResourceThrottle(
      net::URLRequest* request,
      bool is_subresource,
      SafeBrowsingService* service) = 0;

 private:
  static SafeBrowsingResourceThrottleFactory* factory_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingResourceThrottleFactory);
};

#endif  
