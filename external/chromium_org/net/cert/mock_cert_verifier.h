// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_MOCK_CERT_VERIFIER_H_
#define NET_CERT_MOCK_CERT_VERIFIER_H_

#include <list>

#include "net/cert/cert_verifier.h"
#include "net/cert/cert_verify_result.h"

namespace net {

class MockCertVerifier : public CertVerifier {
 public:
  
  
  
  
  
  
  MockCertVerifier();

  virtual ~MockCertVerifier();

  
  virtual int Verify(X509Certificate* cert,
                     const std::string& hostname,
                     int flags,
                     CRLSet* crl_set,
                     CertVerifyResult* verify_result,
                     const CompletionCallback& callback,
                     RequestHandle* out_req,
                     const BoundNetLog& net_log) OVERRIDE;
  virtual void CancelRequest(RequestHandle req) OVERRIDE;

  
  
  void set_default_result(int default_result) {
    default_result_ = default_result;
  }

  
  
  
  
  void AddResultForCert(X509Certificate* cert,
                        const CertVerifyResult& verify_result,
                        int rv);

  
  
  void AddResultForCertAndHost(X509Certificate* cert,
                               const std::string& host_pattern,
                               const CertVerifyResult& verify_result,
                               int rv);

 private:
  struct Rule;
  typedef std::list<Rule> RuleList;

  int default_result_;
  RuleList rules_;
};

}  

#endif  
