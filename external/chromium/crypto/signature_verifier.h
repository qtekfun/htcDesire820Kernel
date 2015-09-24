// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SIGNATURE_VERIFIER_H_
#define CRYPTO_SIGNATURE_VERIFIER_H_
#pragma once

#include "build/build_config.h"

#if defined(USE_NSS)
#include <cryptoht.h>
#elif defined(OS_MACOSX)
#include <Security/cssm.h>
#endif

#include <vector>

#include "base/basictypes.h"

#if defined(OS_WIN)
#include "crypto/scoped_capi_types.h"
#endif

namespace crypto {

class SignatureVerifier {
 public:
  SignatureVerifier();
  ~SignatureVerifier();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool VerifyInit(const uint8* signature_algorithm,
                  int signature_algorithm_len,
                  const uint8* signature,
                  int signature_len,
                  const uint8* public_key_info,
                  int public_key_info_len);

  
  void VerifyUpdate(const uint8* data_part, int data_part_len);

  
  
  
  bool VerifyFinal();

  
  
  
  
  
  
  
  
  

 private:
  void Reset();

  std::vector<uint8> signature_;

#if defined(USE_OPENSSL)
  struct VerifyContext;
  VerifyContext* verify_context_;
#elif defined(USE_NSS)
  VFYContext* vfy_context_;
#elif defined(OS_MACOSX)
  std::vector<uint8> public_key_info_;

  CSSM_CC_HANDLE sig_handle_;

  CSSM_KEY public_key_;
#elif defined(OS_WIN)
  ScopedHCRYPTPROV provider_;

  ScopedHCRYPTHASH hash_object_;

  ScopedHCRYPTKEY public_key_;
#endif
};

}  

#endif  
