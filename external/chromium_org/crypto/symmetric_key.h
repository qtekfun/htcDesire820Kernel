// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SYMMETRIC_KEY_H_
#define CRYPTO_SYMMETRIC_KEY_H_

#include <string>

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

#if defined(NACL_WIN64)
#include "crypto/scoped_capi_types.h"
#elif defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
#include "crypto/scoped_nss_types.h"
#endif

namespace crypto {

class CRYPTO_EXPORT SymmetricKey {
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
#elif defined(NACL_WIN64)
  HCRYPTKEY key() const { return key_.get(); }
#elif defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
  PK11SymKey* key() const { return key_.get(); }
#endif

  
  
  
  bool GetRawKey(std::string* raw_key);

#if defined(OS_CHROMEOS)
  
  static SymmetricKey* CreateFromKey(PK11SymKey* key);
#endif

 private:
#if defined(USE_OPENSSL)
  SymmetricKey() {}
  std::string key_;
#elif defined(NACL_WIN64)
  SymmetricKey(HCRYPTPROV provider, HCRYPTKEY key,
               const void* key_data, size_t key_size_in_bytes);

  ScopedHCRYPTPROV provider_;
  ScopedHCRYPTKEY key_;

  
  
  
  
  
  
  std::string raw_key_;
#elif defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
  explicit SymmetricKey(PK11SymKey* key);
  ScopedPK11SymKey key_;
#endif

  DISALLOW_COPY_AND_ASSIGN(SymmetricKey);
};

}  

#endif  
