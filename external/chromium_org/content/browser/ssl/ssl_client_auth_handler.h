// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SSL_SSL_CLIENT_AUTH_HANDLER_H_
#define CONTENT_BROWSER_SSL_SSL_CLIENT_AUTH_HANDLER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_thread.h"
#include "net/ssl/ssl_cert_request_info.h"

namespace net {
class ClientCertStore;
class HttpNetworkSession;
class URLRequest;
class X509Certificate;
}  

namespace content {

class ResourceContext;

class CONTENT_EXPORT SSLClientAuthHandler
    : public base::RefCountedThreadSafe<
          SSLClientAuthHandler, BrowserThread::DeleteOnIOThread> {
 public:
  SSLClientAuthHandler(scoped_ptr<net::ClientCertStore> client_cert_store,
                       net::URLRequest* request,
                       net::SSLCertRequestInfo* cert_request_info);

  
  
  void SelectCertificate();

  
  
  void OnRequestCancelled();

  
  
  
  void CertificateSelected(net::X509Certificate* cert);

 protected:
  virtual ~SSLClientAuthHandler();

 private:
  friend class base::RefCountedThreadSafe<
      SSLClientAuthHandler, BrowserThread::DeleteOnIOThread>;
  friend class BrowserThread;
  friend class base::DeleteHelper<SSLClientAuthHandler>;

  
  void DidGetClientCerts();

  
  
  void DoCertificateSelected(net::X509Certificate* cert);

  
  void DoSelectCertificate(int render_process_host_id,
                           int render_view_host_id);

  
  net::URLRequest* request_;

  
  const net::HttpNetworkSession* http_network_session_;

  
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info_;

  scoped_ptr<net::ClientCertStore> client_cert_store_;

  DISALLOW_COPY_AND_ASSIGN(SSLClientAuthHandler);
};

}  

#endif  
