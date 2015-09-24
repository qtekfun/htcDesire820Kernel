// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SIGNATURE_VERIFIER_H_
#define CRYPTO_SIGNATURE_VERIFIER_H_

#include "build/build_config.h"

#include <vector>

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

#if defined(USE_OPENSSL)
typedef struct env_md_st EVP_MD;
typedef struct evp_pkey_ctx_st EVP_PKEY_CTX;
#else
typedef struct HASHContextStr HASHContext;
typedef struct SECKEYPublicKeyStr SECKEYPublicKey;
typedef struct VFYContextStr VFYContext;
#endif

namespace crypto {

class CRYPTO_EXPORT SignatureVerifier {
 public:
  
  enum HashAlgorithm {
    SHA1,
    SHA256,
  };

  SignatureVerifier();
  ~SignatureVerifier();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool VerifyInit(const uint8* signature_algorithm,
                  int signature_algorithm_len,
                  const uint8* signature,
                  int signature_len,
                  const uint8* public_key_info,
                  int public_key_info_len);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool VerifyInitRSAPSS(HashAlgorithm hash_alg,
                        HashAlgorithm mask_hash_alg,
                        int salt_len,
                        const uint8* signature,
                        int signature_len,
                        const uint8* public_key_info,
                        int public_key_info_len);

  
  void VerifyUpdate(const uint8* data_part, int data_part_len);

  
  
  
  bool VerifyFinal();

  
  
  
  
  
  
  
  
  

 private:
#if defined(USE_OPENSSL)
  bool CommonInit(const EVP_MD* digest,
                  const uint8* signature,
                  int signature_len,
                  const uint8* public_key_info,
                  int public_key_info_len,
                  EVP_PKEY_CTX** pkey_ctx);
#else
  static SECKEYPublicKey* DecodePublicKeyInfo(const uint8* public_key_info,
                                              int public_key_info_len);
#endif

  void Reset();

  std::vector<uint8> signature_;

#if defined(USE_OPENSSL)
  struct VerifyContext;
  VerifyContext* verify_context_;
#else
  
  VFYContext* vfy_context_;

  
  HashAlgorithm hash_alg_;
  HashAlgorithm mask_hash_alg_;
  unsigned int salt_len_;
  SECKEYPublicKey* public_key_;
  HASHContext* hash_context_;
#endif
};

}  

#endif  
