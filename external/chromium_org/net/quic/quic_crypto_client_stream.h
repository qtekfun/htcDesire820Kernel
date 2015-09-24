// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CRYPTO_CLIENT_STREAM_H_
#define NET_QUIC_QUIC_CRYPTO_CLIENT_STREAM_H_

#include <string>

#include "net/cert/cert_verify_result.h"
#include "net/cert/x509_certificate.h"
#include "net/quic/crypto/proof_verifier.h"
#include "net/quic/crypto/quic_crypto_client_config.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_crypto_stream.h"

namespace net {

class ProofVerifyDetails;
class QuicSession;
class SSLInfo;

namespace test {
class CryptoTestUtils;
}  

class NET_EXPORT_PRIVATE QuicCryptoClientStream : public QuicCryptoStream {
 public:
  QuicCryptoClientStream(const string& server_hostname,
                         QuicSession* session,
                         QuicCryptoClientConfig* crypto_config);
  virtual ~QuicCryptoClientStream();

  
  virtual void OnHandshakeMessage(
      const CryptoHandshakeMessage& message) OVERRIDE;

  
  
  
  virtual bool CryptoConnect();

  
  
  
  int num_sent_client_hellos() const;

  
  bool GetSSLInfo(SSLInfo* ssl_info);

 private:
  
  
  
  class ProofVerifierCallbackImpl : public ProofVerifierCallback {
   public:
    explicit ProofVerifierCallbackImpl(QuicCryptoClientStream* stream);
    virtual ~ProofVerifierCallbackImpl();

    
    virtual void Run(bool ok,
                     const string& error_details,
                     scoped_ptr<ProofVerifyDetails>* details) OVERRIDE;

    
    
    void Cancel();

   private:
    QuicCryptoClientStream* stream_;
  };

  friend class test::CryptoTestUtils;
  friend class ProofVerifierCallbackImpl;

  enum State {
    STATE_IDLE,
    STATE_SEND_CHLO,
    STATE_RECV_REJ,
    STATE_VERIFY_PROOF,
    STATE_VERIFY_PROOF_COMPLETE,
    STATE_RECV_SHLO,
  };

  
  
  void DoHandshakeLoop(const CryptoHandshakeMessage* in);

  State next_state_;
  
  
  int num_client_hellos_;

  QuicCryptoClientConfig* const crypto_config_;

  
  std::string nonce_;
  
  std::string server_hostname_;

  
  uint64 generation_counter_;

  
  
  ProofVerifierCallbackImpl* proof_verify_callback_;

  
  
  
  bool verify_ok_;
  string verify_error_details_;
  scoped_ptr<ProofVerifyDetails> verify_details_;

  
  scoped_ptr<CertVerifyResult> cert_verify_result_;

  DISALLOW_COPY_AND_ASSIGN(QuicCryptoClientStream);
};

}  

#endif  
