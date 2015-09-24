// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SSL_SSL_CERT_ERROR_HANDLER_H_
#define CONTENT_BROWSER_SSL_SSL_CERT_ERROR_HANDLER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "content/browser/ssl/ssl_error_handler.h"
#include "net/ssl/ssl_info.h"

namespace content {

class SSLCertErrorHandler : public SSLErrorHandler {
 public:
  
  SSLCertErrorHandler(const base::WeakPtr<Delegate>& delegate,
                      const GlobalRequestID& id,
                      ResourceType::Type resource_type,
                      const GURL& url,
                      int render_process_id,
                      int render_view_id,
                      const net::SSLInfo& ssl_info,
                      bool fatal);

  virtual SSLCertErrorHandler* AsSSLCertErrorHandler() OVERRIDE;

  
  const net::SSLInfo& ssl_info() const { return ssl_info_; }
  int cert_error() const { return cert_error_; }
  bool fatal() const { return fatal_; }

 protected:
  
  virtual void OnDispatchFailed() OVERRIDE;
  virtual void OnDispatched() OVERRIDE;

 private:
  virtual ~SSLCertErrorHandler();

  
  const net::SSLInfo ssl_info_;
  const int cert_error_;  
  const bool fatal_;  
                      

  DISALLOW_COPY_AND_ASSIGN(SSLCertErrorHandler);
};

}  

#endif  
