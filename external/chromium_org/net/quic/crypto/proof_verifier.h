// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_PROOF_VERIFIER_H_
#define NET_QUIC_CRYPTO_PROOF_VERIFIER_H_

#include <string>
#include <vector>

#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace net {

class CertVerifyResult;

class ProofVerifyDetails {
 public:
  virtual ~ProofVerifyDetails();
};

class NET_EXPORT_PRIVATE ProofVerifierCallback {
 public:
  virtual ~ProofVerifierCallback();

  
  
  
  
  
  
  virtual void Run(bool ok,
                   const std::string& error_details,
                   scoped_ptr<ProofVerifyDetails>* details) = 0;
};

class NET_EXPORT_PRIVATE ProofVerifier {
 public:
  
  enum Status {
    SUCCESS = 0,
    FAILURE = 1,
    
    
    PENDING = 2,
  };

  virtual ~ProofVerifier();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual Status VerifyProof(const std::string& hostname,
                             const std::string& server_config,
                             const std::vector<std::string>& certs,
                             const std::string& signature,
                             std::string* error_details,
                             scoped_ptr<ProofVerifyDetails>* details,
                             ProofVerifierCallback* callback) = 0;
};

}  

#endif  
