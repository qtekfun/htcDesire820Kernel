// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_NET_AW_URL_REQUEST_JOB_FACTORY_H_
#define ANDROID_WEBVIEW_BROWSER_NET_AW_URL_REQUEST_JOB_FACTORY_H_

#include "base/memory/scoped_ptr.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {
class URLRequestJobFactoryImpl;
}  

namespace android_webview {

class AwURLRequestJobFactory : public net::URLRequestJobFactory {
 public:
  AwURLRequestJobFactory();
  virtual ~AwURLRequestJobFactory();

  bool SetProtocolHandler(const std::string& scheme,
                          ProtocolHandler* protocol_handler);

  
  virtual net::URLRequestJob* MaybeCreateJobWithProtocolHandler(
      const std::string& scheme,
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate) const OVERRIDE;
  virtual bool IsHandledProtocol(const std::string& scheme) const OVERRIDE;
  virtual bool IsHandledURL(const GURL& url) const OVERRIDE;
  virtual bool IsSafeRedirectTarget(const GURL& location) const OVERRIDE;

 private:
  
  
  scoped_ptr<net::URLRequestJobFactoryImpl> next_factory_;

  DISALLOW_COPY_AND_ASSIGN(AwURLRequestJobFactory);
};

} 

#endif  
