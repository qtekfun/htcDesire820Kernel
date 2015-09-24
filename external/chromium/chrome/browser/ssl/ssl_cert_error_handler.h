// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_CERT_ERROR_HANDLER_H_
#define CHROME_BROWSER_SSL_SSL_CERT_ERROR_HANDLER_H_
#pragma once

#include <string>

#include "chrome/browser/ssl/ssl_error_handler.h"
#include "net/base/ssl_info.h"

namespace net {
class X509Certificate;
}

class SSLCertErrorHandler : public SSLErrorHandler {
 public:
  
  SSLCertErrorHandler(ResourceDispatcherHost* rdh,
                      net::URLRequest* request,
                      ResourceType::Type resource_type,
                      int cert_error,
                      net::X509Certificate* cert);

  virtual SSLCertErrorHandler* AsSSLCertErrorHandler();

  
  const net::SSLInfo& ssl_info() const { return ssl_info_; }
  int cert_error() const { return cert_error_; }

 protected:
  
  virtual void OnDispatchFailed();
  virtual void OnDispatched();

 private:
  virtual ~SSLCertErrorHandler();

  
  net::SSLInfo ssl_info_;
  const int cert_error_;  

  DISALLOW_COPY_AND_ASSIGN(SSLCertErrorHandler);
};

#endif  
