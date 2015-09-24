// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SIGNATURE_CREATOR_H_
#define CRYPTO_SIGNATURE_CREATOR_H_
#pragma once

#include "build/build_config.h"

#if defined(USE_OPENSSL)
typedef struct env_md_ctx_st EVP_MD_CTX;
#elif defined(USE_NSS)
struct SGNContextStr;
#elif defined(OS_MACOSX)
#include <Security/cssm.h>
#endif

#include <vector>

#include "base/basictypes.h"
#include "crypto/rsa_private_key.h"

#if defined(OS_WIN)
#include "crypto/scoped_capi_types.h"
#endif

namespace crypto {

class SignatureCreator {
 public:
  ~SignatureCreator();

  
  
  static SignatureCreator* Create(RSAPrivateKey* key);

  
  bool Update(const uint8* data_part, int data_part_len);

  
  bool Final(std::vector<uint8>* signature);

 private:
  
  SignatureCreator();

  RSAPrivateKey* key_;

#if defined(USE_OPENSSL)
  EVP_MD_CTX* sign_context_;
#elif defined(USE_NSS)
  SGNContextStr* sign_context_;
#elif defined(OS_MACOSX)
  CSSM_CC_HANDLE sig_handle_;
#elif defined(OS_WIN)
  ScopedHCRYPTHASH hash_object_;
#endif

  DISALLOW_COPY_AND_ASSIGN(SignatureCreator);
};

}  

#endif  
