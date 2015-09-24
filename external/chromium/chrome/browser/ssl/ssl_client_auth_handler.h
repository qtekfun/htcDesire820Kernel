// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_CLIENT_AUTH_HANDLER_H_
#define CHROME_BROWSER_SSL_SSL_CLIENT_AUTH_HANDLER_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "net/base/ssl_cert_request_info.h"

namespace net {
class URLRequest;
class X509Certificate;
}  

class SSLClientAuthHandler
    : public base::RefCountedThreadSafe<SSLClientAuthHandler,
                                        BrowserThread::DeleteOnIOThread> {
 public:
  SSLClientAuthHandler(net::URLRequest* request,
                       net::SSLCertRequestInfo* cert_request_info);

  
  
  
  void SelectCertificate();

  
  
  void OnRequestCancelled();

  
  
  
  void CertificateSelected(net::X509Certificate* cert);

  
  
  
  void CertificateSelectedNoNotify(net::X509Certificate* cert);

  
  net::SSLCertRequestInfo* cert_request_info() { return cert_request_info_; }

 private:
  friend class BrowserThread;
  friend class DeleteTask<SSLClientAuthHandler>;

  virtual ~SSLClientAuthHandler();

  
  
  void DoCertificateSelected(net::X509Certificate* cert);

  
  net::URLRequest* request_;

  
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info_;

  DISALLOW_COPY_AND_ASSIGN(SSLClientAuthHandler);
};

class SSLClientAuthObserver : public NotificationObserver {
 public:
  SSLClientAuthObserver(net::SSLCertRequestInfo* cert_request_info,
                        SSLClientAuthHandler* handler);
  virtual ~SSLClientAuthObserver();

  
  virtual void OnCertSelectedByNotification() = 0;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  void StartObserving();

  
  
  void StopObserving();

 private:
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info_;

  scoped_refptr<SSLClientAuthHandler> handler_;

  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(SSLClientAuthObserver);
};

#endif  
