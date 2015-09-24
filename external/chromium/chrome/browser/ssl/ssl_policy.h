// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_POLICY_H_
#define CHROME_BROWSER_SSL_SSL_POLICY_H_
#pragma once

#include <string>

#include "chrome/browser/ssl/ssl_blocking_page.h"
#include "webkit/glue/resource_type.h"

class NavigationEntry;
class SSLCertErrorHandler;
class SSLPolicyBackend;
class SSLRequestInfo;

class SSLPolicy : public SSLBlockingPage::Delegate {
 public:
  explicit SSLPolicy(SSLPolicyBackend* backend);

  
  void OnCertError(SSLCertErrorHandler* handler);

  void DidRunInsecureContent(NavigationEntry* entry,
                             const std::string& security_origin);

  
  void OnRequestStarted(SSLRequestInfo* info);

  
  
  void UpdateEntry(NavigationEntry* entry, TabContents* tab_contents);

  SSLPolicyBackend* backend() const { return backend_; }

  
  virtual SSLErrorInfo GetSSLErrorInfo(SSLCertErrorHandler* handler);
  virtual void OnDenyCertificate(SSLCertErrorHandler* handler);
  virtual void OnAllowCertificate(SSLCertErrorHandler* handler);

 private:
  
  
  
  
  
  void OnCertErrorInternal(SSLCertErrorHandler* handler,
                           SSLBlockingPage::ErrorLevel error_level);

  
  
  void InitializeEntryIfNeeded(NavigationEntry* entry);

  
  void OriginRanInsecureContent(const std::string& origin, int pid);

  
  SSLPolicyBackend* backend_;

  DISALLOW_COPY_AND_ASSIGN(SSLPolicy);
};

#endif  
