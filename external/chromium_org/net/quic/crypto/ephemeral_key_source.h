// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_EPHEMERAL_KEY_SOURCE_H_
#define NET_QUIC_CRYPTO_EPHEMERAL_KEY_SOURCE_H_

#include <string>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/quic_time.h"

namespace net {

class KeyExchange;
class QuicRandom;

class NET_EXPORT_PRIVATE EphemeralKeySource {
 public:
  virtual ~EphemeralKeySource() {}

  
  
  
  
  
  virtual std::string CalculateForwardSecureKey(
      const KeyExchange* key_exchange,
      QuicRandom* rand,
      QuicTime now,
      base::StringPiece peer_public_value,
      std::string* public_value) = 0;
};

}  

#endif  
