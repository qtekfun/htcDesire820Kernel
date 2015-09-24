// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CONNECT_INTERCEPTOR_H_
#define CHROME_BROWSER_NET_CONNECT_INTERCEPTOR_H_
#pragma once

#include "net/url_request/url_request.h"

namespace chrome_browser_net {

class ConnectInterceptor : public net::URLRequest::Interceptor {
 public:
  
  ConnectInterceptor();
  
  virtual ~ConnectInterceptor();

 protected:
  
  
  virtual net::URLRequestJob* MaybeIntercept(net::URLRequest* request);
  virtual net::URLRequestJob* MaybeInterceptResponse(net::URLRequest* request);
  virtual net::URLRequestJob* MaybeInterceptRedirect(net::URLRequest* request,
                                                     const GURL& location);

 private:
  DISALLOW_COPY_AND_ASSIGN(ConnectInterceptor);
};

}  

#endif  
