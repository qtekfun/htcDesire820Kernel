// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_CLIENT_AUTH_OBSERVER_H_
#define CHROME_BROWSER_SSL_SSL_CLIENT_AUTH_OBSERVER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace net {
class HttpNetworkSession;
class SSLCertRequestInfo;
class X509Certificate;
}

class SSLClientAuthObserver : public content::NotificationObserver {
 public:
  SSLClientAuthObserver(
      const net::HttpNetworkSession* network_session,
      net::SSLCertRequestInfo* cert_request_info,
      const base::Callback<void(net::X509Certificate*)>& callback);
  virtual ~SSLClientAuthObserver();

  
  virtual void OnCertSelectedByNotification() = 0;

  
  
  
  void CertificateSelected(net::X509Certificate* cert);

  
  
  
  
  void StartObserving();

  
  
  void StopObserving();

  net::SSLCertRequestInfo* cert_request_info() const {
    return cert_request_info_.get();
  }

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  const net::HttpNetworkSession* network_session_;
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info_;
  base::Callback<void(net::X509Certificate*)> callback_;
  content::NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(SSLClientAuthObserver);
};

#endif  
