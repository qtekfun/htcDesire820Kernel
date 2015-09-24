// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_HOST_STATE_H_
#define CHROME_BROWSER_SSL_SSL_HOST_STATE_H_
#pragma once

#include <string>
#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"
#include "googleurl/src/gurl.h"
#include "net/base/x509_certificate.h"


class SSLHostState : public base::NonThreadSafe {
 public:
  SSLHostState();
  ~SSLHostState();

  
  void HostRanInsecureContent(const std::string& host, int pid);

  
  bool DidHostRunInsecureContent(const std::string& host, int pid) const;

  
  void DenyCertForHost(net::X509Certificate* cert, const std::string& host);

  
  void AllowCertForHost(net::X509Certificate* cert, const std::string& host);

  
  net::CertPolicy::Judgment QueryPolicy(
      net::X509Certificate* cert, const std::string& host);

 private:
  
  
  typedef std::pair<std::string, int> BrokenHostEntry;

  
  
  
  std::set<BrokenHostEntry> ran_insecure_content_hosts_;

  
  std::map<std::string, net::CertPolicy> cert_policy_for_host_;

  DISALLOW_COPY_AND_ASSIGN(SSLHostState);
};

#endif  
