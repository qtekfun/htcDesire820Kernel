// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CURVE25519_KEY_EXCHANGE_H_
#define NET_QUIC_CRYPTO_CURVE25519_KEY_EXCHANGE_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/key_exchange.h"

namespace net {

class QuicRandom;

class NET_EXPORT_PRIVATE Curve25519KeyExchange : public KeyExchange {
 public:
  virtual ~Curve25519KeyExchange();

  
  
  static Curve25519KeyExchange* New(const base::StringPiece& private_key);

  
  
  static std::string NewPrivateKey(QuicRandom* rand);

  
  virtual KeyExchange* NewKeyPair(QuicRandom* rand) const OVERRIDE;
  virtual bool CalculateSharedKey(const base::StringPiece& peer_public_value,
                                  std::string* shared_key) const OVERRIDE;
  virtual base::StringPiece public_value() const OVERRIDE;
  virtual QuicTag tag() const OVERRIDE;

 private:
  Curve25519KeyExchange();

  uint8 private_key_[32];
  uint8 public_key_[32];
};

}  

#endif  
