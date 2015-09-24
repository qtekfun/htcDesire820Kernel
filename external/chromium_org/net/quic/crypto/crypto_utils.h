// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CRYPTO_UTILS_H_
#define NET_QUIC_CRYPTO_CRYPTO_UTILS_H_

#include <string>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

class QuicTime;
class QuicRandom;
struct QuicCryptoNegotiatedParameters;

class NET_EXPORT_PRIVATE CryptoUtils {
 public:
  enum Perspective {
    SERVER,
    CLIENT,
  };

  
  
  
  
  static void GenerateNonce(QuicWallTime now,
                            QuicRandom* random_generator,
                            base::StringPiece orbit,
                            std::string* nonce);

  
  
  
  
  static bool IsValidSNI(base::StringPiece sni);

  
  
  
  static std::string NormalizeHostname(const char* hostname);

  
  
  
  
  
  
  static bool DeriveKeys(base::StringPiece premaster_secret,
                         QuicTag aead,
                         base::StringPiece client_nonce,
                         base::StringPiece server_nonce,
                         const std::string& hkdf_input,
                         Perspective perspective,
                         CrypterPair* out);
};

}  

#endif  
