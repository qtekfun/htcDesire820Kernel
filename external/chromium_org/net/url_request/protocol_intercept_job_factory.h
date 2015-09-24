// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_PROTOCOL_INTERCEPT_JOB_FACTORY_H_
#define NET_URL_REQUEST_PROTOCOL_INTERCEPT_JOB_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/url_request/url_request_job_factory.h"

class GURL;

namespace net {

class URLRequest;
class URLRequestJob;

class NET_EXPORT ProtocolInterceptJobFactory : public URLRequestJobFactory {
 public:
  ProtocolInterceptJobFactory(scoped_ptr<URLRequestJobFactory> job_factory,
                              scoped_ptr<ProtocolHandler> protocol_handler);
  virtual ~ProtocolInterceptJobFactory();

  
  virtual URLRequestJob* MaybeCreateJobWithProtocolHandler(
      const std::string& scheme,
      URLRequest* request,
      NetworkDelegate* network_delegate) const OVERRIDE;
  virtual bool IsHandledProtocol(const std::string& scheme) const OVERRIDE;
  virtual bool IsHandledURL(const GURL& url) const OVERRIDE;
  virtual bool IsSafeRedirectTarget(const GURL& location) const OVERRIDE;

 private:
  scoped_ptr<URLRequestJobFactory> job_factory_;
  scoped_ptr<ProtocolHandler> protocol_handler_;

  DISALLOW_COPY_AND_ASSIGN(ProtocolInterceptJobFactory);
};

}  

#endif  
