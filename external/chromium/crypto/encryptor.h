// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_ENCRYPTOR_H_
#define CRYPTO_ENCRYPTOR_H_
#pragma once

#include <string>

#include "build/build_config.h"

#if defined(USE_NSS)
#include "crypto/scoped_nss_types.h"
#elif defined(OS_WIN)
#include "crypto/scoped_capi_types.h"
#endif

namespace crypto {

class SymmetricKey;

class Encryptor {
 public:
  enum Mode {
    CBC
  };
  Encryptor();
  virtual ~Encryptor();

  
  
  bool Init(SymmetricKey* key, Mode mode, const std::string& iv);

  
  bool Encrypt(const std::string& plaintext, std::string* ciphertext);

  
  bool Decrypt(const std::string& ciphertext, std::string* plaintext);

  

 private:
  SymmetricKey* key_;
  Mode mode_;

#if defined(USE_OPENSSL)
  bool Crypt(bool encrypt,  
             const std::string& input,
             std::string* output);
  std::string iv_;
#elif defined(USE_NSS)
  ScopedPK11Slot slot_;
  ScopedSECItem param_;
#elif defined(OS_MACOSX)
  bool Crypt(int  op,
             const std::string& input,
             std::string* output);

  std::string iv_;
#elif defined(OS_WIN)
  ScopedHCRYPTKEY capi_key_;
  DWORD block_size_;
#endif
};

}  

#endif  
