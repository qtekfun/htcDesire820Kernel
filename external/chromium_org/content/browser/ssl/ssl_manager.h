// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SSL_SSL_MANAGER_H_
#define CONTENT_BROWSER_SSL_SSL_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/ssl/ssl_error_handler.h"
#include "content/browser/ssl/ssl_policy_backend.h"
#include "content/common/content_export.h"
#include "content/public/browser/global_request_id.h"
#include "net/base/net_errors.h"
#include "net/cert/cert_status_flags.h"
#include "url/gurl.h"

namespace net {
class SSLInfo;
}

namespace content {
class BrowserContext;
class NavigationEntryImpl;
class NavigationControllerImpl;
class SSLPolicy;
struct LoadCommittedDetails;
struct LoadFromMemoryCacheDetails;
struct ResourceRedirectDetails;
struct ResourceRequestDetails;


class SSLManager {
 public:
  
  
  
  
  
  
  static void OnSSLCertificateError(
      const base::WeakPtr<SSLErrorHandler::Delegate>& delegate,
      const GlobalRequestID& id,
      ResourceType::Type resource_type,
      const GURL& url,
      int render_process_id,
      int render_view_id,
      const net::SSLInfo& ssl_info,
      bool fatal);

  
  static void NotifySSLInternalStateChanged(BrowserContext* context);

  
  
  explicit SSLManager(NavigationControllerImpl* controller);
  virtual ~SSLManager();

  SSLPolicy* policy() { return policy_.get(); }
  SSLPolicyBackend* backend() { return &backend_; }

  
  
  NavigationControllerImpl* controller() { return controller_; }

  void DidCommitProvisionalLoad(const LoadCommittedDetails& details);
  void DidLoadFromMemoryCache(const LoadFromMemoryCacheDetails& details);
  void DidStartResourceResponse(const ResourceRequestDetails& details);
  void DidReceiveResourceRedirect(const ResourceRedirectDetails& details);

  
  void DidDisplayInsecureContent();
  void DidRunInsecureContent(const std::string& security_origin);

 private:
  
  void UpdateEntry(NavigationEntryImpl* entry);

  
  SSLPolicyBackend backend_;

  
  scoped_ptr<SSLPolicy> policy_;

  
  
  NavigationControllerImpl* controller_;

  DISALLOW_COPY_AND_ASSIGN(SSLManager);
};

}  

#endif  
