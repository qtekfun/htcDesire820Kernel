// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_P224_SPAKE_H_
#define CRYPTO_P224_SPAKE_H_

#include <base/strings/string_piece.h>
#include <crypto/p224.h>
#include <crypto/sha2.h>

namespace crypto {

class CRYPTO_EXPORT P224EncryptedKeyExchange {
 public:
  enum Result {
    kResultPending,
    kResultFailed,
    kResultSuccess,
  };

  
  
  
  enum PeerType {
    kPeerTypeClient,
    kPeerTypeServer,
  };

  
  
  
  
  P224EncryptedKeyExchange(PeerType peer_type,
                           const base::StringPiece& password);

  
  
  const std::string& GetMessage();

  
  
  Result ProcessMessage(const base::StringPiece& message);

  
  
  const std::string& error() const;

  
  
  const std::string& GetKey();

 private:
  
  
  enum State {
    kStateInitial,
    kStateRecvDH,
    kStateSendHash,
    kStateRecvHash,
    kStateDone,
  };

  State state_;
  const bool is_server_;
  
  std::string next_message_;
  std::string error_;

  
  
  void CalculateHash(
      PeerType peer_type,
      const std::string& client_masked_dh,
      const std::string& server_masked_dh,
      const std::string& k,
      uint8* out_digest);

  
  
  uint8 x_[p224::kScalarBytes];
  
  
  uint8 pw_[p224::kScalarBytes];
  
  
  uint8 expected_authenticator_[kSHA256Length];

  std::string key_;
};

}  

#endif  
