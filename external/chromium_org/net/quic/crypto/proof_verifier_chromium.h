// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_PROOF_VERIFIER_CHROMIUM_H_
#define NET_QUIC_CRYPTO_PROOF_VERIFIER_CHROMIUM_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/cert/cert_verify_result.h"
#include "net/cert/x509_certificate.h"
#include "net/quic/crypto/proof_verifier.h"

namespace net {

class CertVerifier;
class SingleRequestCertVerifier;

struct ProofVerifyDetailsChromium : public ProofVerifyDetails {
 public:
  CertVerifyResult cert_verify_result;
};

class NET_EXPORT_PRIVATE ProofVerifierChromium : public ProofVerifier {
 public:
  ProofVerifierChromium(CertVerifier* cert_verifier,
                        const BoundNetLog& net_log);
  virtual ~ProofVerifierChromium();

  
  virtual Status VerifyProof(const std::string& hostname,
                             const std::string& server_config,
                             const std::vector<std::string>& certs,
                             const std::string& signature,
                             std::string* error_details,
                             scoped_ptr<ProofVerifyDetails>* details,
                             ProofVerifierCallback* callback) OVERRIDE;

 private:
  enum State {
    STATE_NONE,
    STATE_VERIFY_CERT,
    STATE_VERIFY_CERT_COMPLETE,
  };

  int DoLoop(int last_io_result);
  void OnIOComplete(int result);
  int DoVerifyCert(int result);
  int DoVerifyCertComplete(int result);

  bool VerifySignature(const std::string& signed_data,
                       const std::string& signature,
                       const std::string& cert);

  
  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;

  
  std::string hostname_;

  scoped_ptr<ProofVerifierCallback> callback_;
  scoped_ptr<ProofVerifyDetailsChromium> verify_details_;
  std::string error_details_;

  
  scoped_refptr<X509Certificate> cert_;

  State next_state_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(ProofVerifierChromium);
};

}  

#endif  
