// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_SCOPED_EVP_CIPHER_CTX_H_
#define NET_QUIC_CRYPTO_SCOPED_EVP_CIPHER_CTX_H_

typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;

namespace net {

class ScopedEVPCipherCtx {
 public:
  ScopedEVPCipherCtx();
  ~ScopedEVPCipherCtx();

  EVP_CIPHER_CTX* get() const;

 private:
  EVP_CIPHER_CTX* const ctx_;
};

}  

#endif  
