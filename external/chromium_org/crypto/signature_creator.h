// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SIGNATURE_CREATOR_H_
#define CRYPTO_SIGNATURE_CREATOR_H_

#include "build/build_config.h"

#include <vector>

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

#if defined(USE_OPENSSL)
typedef struct env_md_ctx_st EVP_MD_CTX;
#elif defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
struct SGNContextStr;
#endif

namespace crypto {

class RSAPrivateKey;

class CRYPTO_EXPORT SignatureCreator {
 public:
  ~SignatureCreator();

  
  
  static SignatureCreator* Create(RSAPrivateKey* key);

  
  
  static bool Sign(RSAPrivateKey* key,
                   const uint8* data,
                   int data_len,
                   std::vector<uint8>* signature);

  
  bool Update(const uint8* data_part, int data_part_len);

  
  bool Final(std::vector<uint8>* signature);

 private:
  
  SignatureCreator();

  RSAPrivateKey* key_;

#if defined(USE_OPENSSL)
  EVP_MD_CTX* sign_context_;
#elif defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
  SGNContextStr* sign_context_;
#endif

  DISALLOW_COPY_AND_ASSIGN(SignatureCreator);
};

}  

#endif  
