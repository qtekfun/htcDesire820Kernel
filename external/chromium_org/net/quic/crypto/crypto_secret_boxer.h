// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CRYPTO_SECRET_BOXER_H_
#define NET_QUIC_CRYPTO_CRYPTO_SECRET_BOXER_H_

#include <string>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace net {

class QuicRandom;

class NET_EXPORT_PRIVATE CryptoSecretBoxer {
 public:
  
  static size_t GetKeySize();

  
  
  void SetKey(base::StringPiece key);

  
  
  
  std::string Box(QuicRandom* rand, base::StringPiece plaintext) const;

  
  
  
  
  
  bool Unbox(base::StringPiece ciphertext,
             std::string* out_storage,
             base::StringPiece* out) const;

 private:
  std::string key_;
};

}  

#endif  
