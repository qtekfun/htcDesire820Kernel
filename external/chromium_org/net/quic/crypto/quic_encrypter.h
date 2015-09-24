// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_QUIC_ENCRYPTER_H_
#define NET_QUIC_CRYPTO_QUIC_ENCRYPTER_H_

#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicEncrypter {
 public:
  virtual ~QuicEncrypter() {}

  static QuicEncrypter* Create(QuicTag algorithm);

  
  
  
  
  virtual bool SetKey(base::StringPiece key) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool SetNoncePrefix(base::StringPiece nonce_prefix) = 0;

  
  
  
  
  
  virtual bool Encrypt(base::StringPiece nonce,
                       base::StringPiece associated_data,
                       base::StringPiece plaintext,
                       unsigned char* output) = 0;

  
  
  
  
  virtual QuicData* EncryptPacket(QuicPacketSequenceNumber sequence_number,
                                  base::StringPiece associated_data,
                                  base::StringPiece plaintext) = 0;

  
  
  
  
  

  
  virtual size_t GetKeySize() const = 0;
  
  virtual size_t GetNoncePrefixSize() const = 0;

  
  
  virtual size_t GetMaxPlaintextSize(size_t ciphertext_size) const = 0;

  
  
  virtual size_t GetCiphertextSize(size_t plaintext_size) const = 0;

  
  virtual base::StringPiece GetKey() const = 0;
  virtual base::StringPiece GetNoncePrefix() const = 0;
};

}  

#endif  
