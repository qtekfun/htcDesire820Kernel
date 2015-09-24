// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_MANAGER_H_
#define CHROME_BROWSER_SSL_SSL_MANAGER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/ssl/ssl_policy_backend.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "net/base/net_errors.h"

class LoadFromMemoryCacheDetails;
class NavigationController;
class NavigationEntry;
class ProvisionalLoadDetails;
class ResourceDispatcherHost;
class ResourceRedirectDetails;
class ResourceRequestDetails;
class SSLPolicy;

namespace net {
class URLRequest;
}  


class SSLManager : public NotificationObserver {
 public:
  
  
  
  
  
  
  static void OnSSLCertificateError(ResourceDispatcherHost* resource_dispatcher,
                                    net::URLRequest* request,
                                    int cert_error,
                                    net::X509Certificate* cert);

  
  
  static void NotifySSLInternalStateChanged();

  
  static std::string SerializeSecurityInfo(int cert_id,
                                           int cert_status,
                                           int security_bits,
                                           int connection_status);
  static bool DeserializeSecurityInfo(const std::string& state,
                                      int* cert_id,
                                      int* cert_status,
                                      int* security_bits,
                                      int* connection_status);

  
  static string16 GetEVCertName(const net::X509Certificate& cert);

  
  
  explicit SSLManager(NavigationController* controller);
  ~SSLManager();

  SSLPolicy* policy() { return policy_.get(); }
  SSLPolicyBackend* backend() { return &backend_; }

  
  
  NavigationController* controller() { return controller_; }

  
  
  
  void DidCommitProvisionalLoad(const NotificationDetails& details);

  
  void DidRunInsecureContent(const std::string& security_origin);

  
  bool ProcessedSSLErrorFromRequest() const;

  
  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  
  
  
  void DidLoadFromMemoryCache(LoadFromMemoryCacheDetails* details);
  void DidStartResourceResponse(ResourceRequestDetails* details);
  void DidReceiveResourceRedirect(ResourceRedirectDetails* details);
  void DidChangeSSLInternalState();

  
  void UpdateEntry(NavigationEntry* entry);

  
  SSLPolicyBackend backend_;

  
  scoped_ptr<SSLPolicy> policy_;

  
  
  NavigationController* controller_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(SSLManager);
};

#endif  
