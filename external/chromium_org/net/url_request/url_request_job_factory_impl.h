// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_JOB_FACTORY_IMPL_H_
#define NET_URL_REQUEST_URL_REQUEST_JOB_FACTORY_IMPL_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/net_export.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {

class NET_EXPORT URLRequestJobFactoryImpl : public URLRequestJobFactory {
 public:
  URLRequestJobFactoryImpl();
  virtual ~URLRequestJobFactoryImpl();

  
  
  
  bool SetProtocolHandler(const std::string& scheme,
                          ProtocolHandler* protocol_handler);

  
  virtual URLRequestJob* MaybeCreateJobWithProtocolHandler(
      const std::string& scheme,
      URLRequest* request,
      NetworkDelegate* network_delegate) const OVERRIDE;
  virtual bool IsHandledProtocol(const std::string& scheme) const OVERRIDE;
  virtual bool IsHandledURL(const GURL& url) const OVERRIDE;
  virtual bool IsSafeRedirectTarget(const GURL& location) const OVERRIDE;

 private:
  typedef std::map<std::string, ProtocolHandler*> ProtocolHandlerMap;

  ProtocolHandlerMap protocol_handler_map_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestJobFactoryImpl);
};

}  

#endif  
