// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_P256_KEY_EXCHANGE_H_
#define NET_QUIC_CRYPTO_P256_KEY_EXCHANGE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/key_exchange.h"

#if defined(USE_OPENSSL)
#include "crypto/openssl_util.h"
typedef struct ec_key_st EC_KEY;
extern "C" void EC_KEY_free(EC_KEY* key);
#else
#include "crypto/ec_private_key.h"
#include "crypto/scoped_nss_types.h"
#endif

namespace net {

class NET_EXPORT_PRIVATE P256KeyExchange : public KeyExchange {
 public:
  virtual ~P256KeyExchange();

  
  
  static P256KeyExchange* New(base::StringPiece private_key);

  
  
  
  static std::string NewPrivateKey();

  
  virtual KeyExchange* NewKeyPair(QuicRandom* rand) const OVERRIDE;
  virtual bool CalculateSharedKey(const base::StringPiece& peer_public_value,
                                  std::string* shared_key) const OVERRIDE;
  virtual base::StringPiece public_value() const OVERRIDE;
  virtual QuicTag tag() const OVERRIDE;

 private:
  enum {
    
    kP256FieldBytes = 32,
    
    
    
    kUncompressedP256PointBytes = 1 + 2 * kP256FieldBytes,
    
    kUncompressedECPointForm = 0x04,
  };

#if defined(USE_OPENSSL)
  
  
  P256KeyExchange(EC_KEY* private_key, const uint8* public_key);

  crypto::ScopedOpenSSL<EC_KEY, EC_KEY_free> private_key_;
#else
  
  
  P256KeyExchange(crypto::ECPrivateKey* key_pair, const uint8* public_key);

  scoped_ptr<crypto::ECPrivateKey> key_pair_;
#endif
  
  uint8 public_key_[kUncompressedP256PointBytes];
};

}  
#endif  

