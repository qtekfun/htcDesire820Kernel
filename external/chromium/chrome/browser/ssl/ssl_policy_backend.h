// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_POLICY_BACKEND_H_
#define CHROME_BROWSER_SSL_SSL_POLICY_BACKEND_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "net/base/x509_certificate.h"

class NavigationController;
class SSLHostState;

class SSLPolicyBackend {
 public:
  explicit SSLPolicyBackend(NavigationController* controller);

  
  void HostRanInsecureContent(const std::string& host, int pid);

  
  bool DidHostRunInsecureContent(const std::string& host, int pid) const;

  
  void DenyCertForHost(net::X509Certificate* cert, const std::string& host);

  
  void AllowCertForHost(net::X509Certificate* cert, const std::string& host);

  
  net::CertPolicy::Judgment QueryPolicy(
      net::X509Certificate* cert, const std::string& host);

 private:
  
  SSLHostState* ssl_host_state_;

  DISALLOW_COPY_AND_ASSIGN(SSLPolicyBackend);
};

#endif  
