// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_QUIC_CRYPTO_CLIENT_CONFIG_H_
#define NET_QUIC_CRYPTO_QUIC_CRYPTO_CLIENT_CONFIG_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicCryptoClientConfig : public QuicCryptoConfig {
 public:
  
  
  
  class NET_EXPORT_PRIVATE CachedState {
   public:
    CachedState();
    ~CachedState();

    
    
    
    bool IsComplete(QuicWallTime now) const;

    
    
    
    const CryptoHandshakeMessage* GetServerConfig() const;

    
    
    
    QuicErrorCode SetServerConfig(base::StringPiece server_config,
                                  QuicWallTime now,
                                  std::string* error_details);

    
    void InvalidateServerConfig();

    
    void SetProof(const std::vector<std::string>& certs,
                  base::StringPiece signature);

    
    void ClearProof();

    
    
    
    void SetProofValid();

    
    
    
    void SetProofInvalid();

    const std::string& server_config() const;
    const std::string& source_address_token() const;
    const std::vector<std::string>& certs() const;
    const std::string& signature() const;
    bool proof_valid() const;
    uint64 generation_counter() const;
    const ProofVerifyDetails* proof_verify_details() const;

    void set_source_address_token(base::StringPiece token);

    
    void SetProofVerifyDetails(ProofVerifyDetails* details);

    
    
    
    
    void InitializeFrom(const CachedState& other);

   private:
    std::string server_config_id_;      
    std::string server_config_;         
    std::string source_address_token_;  
    std::vector<std::string> certs_;    
                                        
    std::string server_config_sig_;     
    bool server_config_valid_;          
                                        
                                        
    
    
    
    uint64 generation_counter_;

    scoped_ptr<ProofVerifyDetails> proof_verify_details_;

    
    mutable scoped_ptr<CryptoHandshakeMessage> scfg_;

    DISALLOW_COPY_AND_ASSIGN(CachedState);
  };

  QuicCryptoClientConfig();
  ~QuicCryptoClientConfig();

  
  void SetDefaults();

  
  
  CachedState* LookupOrCreate(const std::string& server_hostname);

  
  
  
  
  
  
  
  void FillInchoateClientHello(const std::string& server_hostname,
                               const QuicVersion preferred_version,
                               const CachedState* cached,
                               QuicCryptoNegotiatedParameters* out_params,
                               CryptoHandshakeMessage* out) const;

  
  
  
  
  
  
  
  
  
  
  QuicErrorCode FillClientHello(const std::string& server_hostname,
                                QuicGuid guid,
                                const QuicVersion preferred_version,
                                const CachedState* cached,
                                QuicWallTime now,
                                QuicRandom* rand,
                                QuicCryptoNegotiatedParameters* out_params,
                                CryptoHandshakeMessage* out,
                                std::string* error_details) const;

  
  
  
  
  
  
  QuicErrorCode ProcessRejection(const CryptoHandshakeMessage& rej,
                                 QuicWallTime now,
                                 CachedState* cached,
                                 QuicCryptoNegotiatedParameters* out_params,
                                 std::string* error_details);

  
  
  
  
  
  
  
  
  QuicErrorCode ProcessServerHello(const CryptoHandshakeMessage& server_hello,
                                   QuicGuid guid,
                                   const QuicVersionVector& negotiated_versions,
                                   CachedState* cached,
                                   QuicCryptoNegotiatedParameters* out_params,
                                   std::string* error_details);

  ProofVerifier* proof_verifier() const;

  
  
  
  
  void SetProofVerifier(ProofVerifier* verifier);

  ChannelIDSigner* channel_id_signer() const;

  
  
  
  void SetChannelIDSigner(ChannelIDSigner* signer);

  
  
  
  
  void InitializeFrom(const std::string& server_hostname,
                      const std::string& canonical_server_hostname,
                      QuicCryptoClientConfig* canonical_crypto_config);

 private:
  
  
  std::map<std::string, CachedState*> cached_states_;

  scoped_ptr<ProofVerifier> proof_verifier_;
  scoped_ptr<ChannelIDSigner> channel_id_signer_;

  DISALLOW_COPY_AND_ASSIGN(QuicCryptoClientConfig);
};

}  

#endif  
