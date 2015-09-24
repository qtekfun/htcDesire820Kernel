// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SSL_SSL_POLICY_H_
#define CONTENT_BROWSER_SSL_SSL_POLICY_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "webkit/common/resource_type.h"

namespace content {
class NavigationEntryImpl;
class SSLCertErrorHandler;
class SSLPolicyBackend;
class SSLRequestInfo;
class WebContentsImpl;

class SSLPolicy {
 public:
  explicit SSLPolicy(SSLPolicyBackend* backend);

  
  void OnCertError(SSLCertErrorHandler* handler);

  void DidRunInsecureContent(NavigationEntryImpl* entry,
                             const std::string& security_origin);

  
  void OnRequestStarted(SSLRequestInfo* info);

  
  
  void UpdateEntry(NavigationEntryImpl* entry,
                   WebContentsImpl* web_contents);

  SSLPolicyBackend* backend() const { return backend_; }

 private:
  
  void OnAllowCertificate(scoped_refptr<SSLCertErrorHandler> handler,
                          bool allow);

  
  
  
  
  
  
  
  void OnCertErrorInternal(SSLCertErrorHandler* handler,
                           bool overridable,
                           bool strict_enforcement);

  
  
  void InitializeEntryIfNeeded(NavigationEntryImpl* entry);

  
  void OriginRanInsecureContent(const std::string& origin, int pid);

  
  SSLPolicyBackend* backend_;

  DISALLOW_COPY_AND_ASSIGN(SSLPolicy);
};

}  

#endif  
