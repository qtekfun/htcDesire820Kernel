// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_SINGLE_REQUEST_CERT_VERIFIER_H_
#define NET_CERT_SINGLE_REQUEST_CERT_VERIFIER_H_

#include "net/cert/cert_verifier.h"

namespace net {

class SingleRequestCertVerifier {
 public:
  
  explicit SingleRequestCertVerifier(CertVerifier* cert_verifier);

  
  
  
  ~SingleRequestCertVerifier();

  
  
  int Verify(X509Certificate* cert,
             const std::string& hostname,
             int flags,
             CRLSet* crl_set,
             CertVerifyResult* verify_result,
             const CompletionCallback& callback,
             const BoundNetLog& net_log);

 private:
  
  
  void OnVerifyCompletion(int result);

  
  CertVerifier* const cert_verifier_;

  
  CertVerifier::RequestHandle cur_request_;
  CompletionCallback cur_request_callback_;

  DISALLOW_COPY_AND_ASSIGN(SingleRequestCertVerifier);
};

}  

#endif  
