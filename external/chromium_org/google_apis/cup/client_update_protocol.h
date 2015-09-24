// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_CUP_CLIENT_UPDATE_PROTOCOL_H_
#define GOOGLE_APIS_CUP_CLIENT_UPDATE_PROTOCOL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"

#if defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
typedef struct SECKEYPublicKeyStr SECKEYPublicKey;
#endif

class ClientUpdateProtocol {
 public:
  ~ClientUpdateProtocol();

  
  
  
  static scoped_ptr<ClientUpdateProtocol> Create(
      int key_version, const base::StringPiece& public_key);

  
  
  std::string GetVersionedSecret() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool SignRequest(const base::StringPiece& url,
                   const base::StringPiece& request_body,
                   std::string* client_proof);

  
  
  
  
  
  
  
  bool ValidateResponse(const base::StringPiece& response_body,
                        const base::StringPiece& server_cookie,
                        const base::StringPiece& server_proof);

 private:
  friend class CupTest;

  explicit ClientUpdateProtocol(int key_version);

  
  
  bool LoadPublicKey(const base::StringPiece& public_key);

  
  size_t PublicKeyLength();

  
  
  
  bool EncryptKeySource(const std::vector<uint8>& key_source);

  
  
  bool BuildRandomSharedKey();

  
  
  bool SetSharedKeyForTesting(const base::StringPiece& fixed_key_source);

  
  
  bool DeriveSharedKey(const std::vector<uint8>& source);

  
  
  
  int pub_key_version_;

  
  
  
  
  std::vector<uint8> shared_key_;

  
  
  
  
  
  std::vector<uint8> encrypted_key_source_;

  
  
  
  
  
  std::vector<uint8> client_challenge_hash_;

  
  
#if defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
  SECKEYPublicKey* public_key_;
#endif

  DISALLOW_IMPLICIT_CONSTRUCTORS(ClientUpdateProtocol);
};

#endif  

