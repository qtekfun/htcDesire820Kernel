// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_ADD_CERT_HANDLER_H_
#define CHROME_BROWSER_SSL_SSL_ADD_CERT_HANDLER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace net {
class URLRequest;
class X509Certificate;
}

class SSLAddCertHandler : public base::RefCountedThreadSafe<SSLAddCertHandler> {
 public:
  SSLAddCertHandler(net::URLRequest* request, net::X509Certificate* cert,
                    int render_process_host_id, int render_view_id);

  net::X509Certificate* cert() { return cert_.get(); }

  int network_request_id() const { return network_request_id_; }

  
  
  void Finished(bool add_cert);

 private:
  friend class base::RefCountedThreadSafe<SSLAddCertHandler>;
  virtual ~SSLAddCertHandler();

  
  void Run();

  
  
  void AskToAddCert();

  
  void CallVerifyClientCertificateError(int cert_error);
  void CallAddClientCertificate(bool add_cert, int cert_error);

  
  scoped_refptr<net::X509Certificate> cert_;

  
  int network_request_id_;
  
  int render_process_host_id_;
  
  int render_view_id_;

  DISALLOW_COPY_AND_ASSIGN(SSLAddCertHandler);
};

#endif  
