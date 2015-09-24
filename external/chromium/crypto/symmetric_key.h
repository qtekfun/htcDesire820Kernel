// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SYMMETRIC_KEY_H_
#define CRYPTO_SYMMETRIC_KEY_H_
#pragma once

#include <string>

#include "base/basictypes.h"

#if defined(USE_NSS)
#include "crypto/scoped_nss_types.h"
#elif defined(OS_MACOSX)
#include <Security/cssmtype.h>
#elif defined(OS_WIN)
#include "crypto/scoped_capi_types.h"
#endif

namespace crypto {

class SymmetricKey {
 public:
  
  
  enum Algorithm {
    AES,
    HMAC_SHA1,
  };

  virtual ~SymmetricKey();

  
  
  
  static SymmetricKey* GenerateRandomKey(Algorithm algorithm,
                                         size_t key_size_in_bits);

  
  
  
  
  static SymmetricKey* DeriveKeyFromPassword(Algorithm algorithm,
                                             const std::string& password,
                                             const std::string& salt,
                                             size_t iterations,
                                             size_t key_size_in_bits);

  
  
  
  
  static SymmetricKey* Import(Algorithm algorithm, const std::string& raw_key);

#if defined(USE_OPENSSL)
  const std::string& key() { return key_; }
#elif defined(USE_NSS)
  PK11SymKey* key() const { return key_.get(); }
#elif defined(OS_MACOSX)
  CSSM_DATA cssm_data() const;
#elif defined(OS_WIN)
  HCRYPTKEY key() const { return key_.get(); }
#endif

  
  
  
  bool GetRawKey(std::string* raw_key);

 private:
#if defined(USE_OPENSSL)
  SymmetricKey() {}
  std::string key_;
#elif defined(USE_NSS)
  explicit SymmetricKey(PK11SymKey* key);
  ScopedPK11SymKey key_;
#elif defined(OS_MACOSX)
  SymmetricKey(const void* key_data, size_t key_size_in_bits);
  std::string key_;
#elif defined(OS_WIN)
  SymmetricKey(HCRYPTPROV provider, HCRYPTKEY key,
               const void* key_data, size_t key_size_in_bytes);

  ScopedHCRYPTPROV provider_;
  ScopedHCRYPTKEY key_;

  
  
  
  
  
  
  std::string raw_key_;
#endif

  DISALLOW_COPY_AND_ASSIGN(SymmetricKey);
};

}  

#endif  
