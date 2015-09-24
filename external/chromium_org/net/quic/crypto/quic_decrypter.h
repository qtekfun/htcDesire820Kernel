// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_QUIC_DECRYPTER_H_
#define NET_QUIC_CRYPTO_QUIC_DECRYPTER_H_

#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicDecrypter {
 public:
  virtual ~QuicDecrypter() {}

  static QuicDecrypter* Create(QuicTag algorithm);

  
  
  
  
  virtual bool SetKey(base::StringPiece key) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool SetNoncePrefix(base::StringPiece nonce_prefix) = 0;

  
  
  
  
  // successful return, the true length of the plaintext will be written to
  
  virtual bool Decrypt(base::StringPiece nonce,
                       base::StringPiece associated_data,
                       base::StringPiece ciphertext,
                       unsigned char* output,
                       size_t* output_length) = 0;

  
  
  
  
  
  
  virtual QuicData* DecryptPacket(QuicPacketSequenceNumber sequence_number,
                                  base::StringPiece associated_data,
                                  base::StringPiece ciphertext) = 0;

  
  virtual base::StringPiece GetKey() const = 0;
  virtual base::StringPiece GetNoncePrefix() const = 0;
};

}  

#endif  
