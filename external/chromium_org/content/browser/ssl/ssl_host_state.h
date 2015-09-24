// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SSL_SSL_HOST_STATE_H_
#define CONTENT_BROWSER_SSL_SSL_HOST_STATE_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/supports_user_data.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"
#include "net/cert/cert_status_flags.h"
#include "net/cert/x509_certificate.h"

namespace content {
class BrowserContext;


class CONTENT_EXPORT SSLHostState
    : NON_EXPORTED_BASE(base::SupportsUserData::Data),
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  static SSLHostState* GetFor(BrowserContext* browser_context);

  SSLHostState();
  virtual ~SSLHostState();

  
  void HostRanInsecureContent(const std::string& host, int pid);

  
  bool DidHostRunInsecureContent(const std::string& host, int pid) const;

  
  
  void DenyCertForHost(net::X509Certificate* cert,
                       const std::string& host,
                       net::CertStatus error);

  
  
  void AllowCertForHost(net::X509Certificate* cert,
                        const std::string& host,
                        net::CertStatus error);

  
  void Clear();

  
  net::CertPolicy::Judgment QueryPolicy(net::X509Certificate* cert,
                                        const std::string& host,
                                        net::CertStatus error);

 private:
  
  
  typedef std::pair<std::string, int> BrokenHostEntry;

  
  
  
  std::set<BrokenHostEntry> ran_insecure_content_hosts_;

  
  std::map<std::string, net::CertPolicy> cert_policy_for_host_;

  DISALLOW_COPY_AND_ASSIGN(SSLHostState);
};

}  

#endif  
