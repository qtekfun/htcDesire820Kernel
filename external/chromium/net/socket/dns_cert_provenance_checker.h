// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_DNS_CERT_PROVENANCE_CHECKER_H
#define NET_SOCKET_DNS_CERT_PROVENANCE_CHECKER_H

#include <string>
#include <vector>

#include "base/string_piece.h"

namespace net {

class DnsRRResolver;

class DnsCertProvenanceChecker {
 public:
  class Delegate {
   public:
    virtual ~Delegate();

    virtual void OnDnsCertLookupFailed(
        const std::string& hostname,
        const std::vector<std::string>& der_certs) = 0;
  };

  virtual ~DnsCertProvenanceChecker();

  virtual void Shutdown() = 0;

  
  
  virtual void DoAsyncVerification(
      const std::string& hostname,
      const std::vector<base::StringPiece>& der_certs) = 0;


 protected:
  
  
  
  static void DoAsyncLookup(
      const std::string& hostname,
      const std::vector<base::StringPiece>& der_certs,
      DnsRRResolver* dnsrr_resolver,
      Delegate* delegate);

  
  
  
  
  static std::string BuildEncryptedReport(
      const std::string& hostname,
      const std::vector<std::string>& der_certs);
};

}  

#endif  
