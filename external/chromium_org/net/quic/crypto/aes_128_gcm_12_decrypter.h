// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_AES_128_GCM_12_DECRYPTER_H_
#define NET_QUIC_CRYPTO_AES_128_GCM_12_DECRYPTER_H_

#include <string>

#include "base/compiler_specific.h"
#include "net/quic/crypto/quic_decrypter.h"

#if defined(USE_OPENSSL)
#include "net/quic/crypto/scoped_evp_cipher_ctx.h"
#endif

namespace net {

namespace test {
class Aes128Gcm12DecrypterPeer;
}  

class NET_EXPORT_PRIVATE Aes128Gcm12Decrypter : public QuicDecrypter {
 public:
  enum {
    
    kAuthTagSize = 12,
  };

  Aes128Gcm12Decrypter();
  virtual ~Aes128Gcm12Decrypter();

  
  static bool IsSupported();

  
  virtual bool SetKey(base::StringPiece key) OVERRIDE;
  virtual bool SetNoncePrefix(base::StringPiece nonce_prefix) OVERRIDE;
  virtual bool Decrypt(base::StringPiece nonce,
                       base::StringPiece associated_data,
                       base::StringPiece ciphertext,
                       unsigned char* output,
                       size_t* output_length) OVERRIDE;
  virtual QuicData* DecryptPacket(QuicPacketSequenceNumber sequence_number,
                                  base::StringPiece associated_data,
                                  base::StringPiece ciphertext) OVERRIDE;
  virtual base::StringPiece GetKey() const OVERRIDE;
  virtual base::StringPiece GetNoncePrefix() const OVERRIDE;

 private:
  
  unsigned char key_[16];
  
  unsigned char nonce_prefix_[4];

#if defined(USE_OPENSSL)
  
  
  ScopedEVPCipherCtx ctx_;
#endif
};

}  

#endif  
