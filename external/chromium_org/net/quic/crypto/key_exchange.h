// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_KEY_EXCHANGE_H_
#define NET_QUIC_CRYPTO_KEY_EXCHANGE_H_

#include <string>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_protocol.h"

namespace net {

class QuicRandom;

class NET_EXPORT_PRIVATE KeyExchange {
 public:
  virtual ~KeyExchange() {}

  
  
  
  virtual KeyExchange* NewKeyPair(QuicRandom* rand) const = 0;

  
  
  
  virtual bool CalculateSharedKey(const base::StringPiece& peer_public_value,
                                  std::string* shared_key) const = 0;

  
  
  
  
  virtual base::StringPiece public_value() const = 0;

  
  virtual QuicTag tag() const = 0;
};

}  

#endif  
