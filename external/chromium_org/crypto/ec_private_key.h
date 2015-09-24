// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_EC_PRIVATE_KEY_H_
#define CRYPTO_EC_PRIVATE_KEY_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "build/build_config.h"
#include "crypto/crypto_export.h"

#if defined(USE_OPENSSL)
typedef struct evp_pkey_st EVP_PKEY;
#else
typedef struct CERTSubjectPublicKeyInfoStr CERTSubjectPublicKeyInfo;
typedef struct PK11SlotInfoStr PK11SlotInfo;
typedef struct SECKEYPrivateKeyStr SECKEYPrivateKey;
typedef struct SECKEYPublicKeyStr SECKEYPublicKey;
#endif

namespace crypto {

class CRYPTO_EXPORT ECPrivateKey {
 public:
  ~ECPrivateKey();

  
  static bool IsSupported();

  
  
  
  static ECPrivateKey* Create();

#if defined(USE_NSS)
  
  
  
  static ECPrivateKey* CreateSensitive(PK11SlotInfo* slot);
#endif

  
  
  
  
  static ECPrivateKey* CreateFromEncryptedPrivateKeyInfo(
      const std::string& password,
      const std::vector<uint8>& encrypted_private_key_info,
      const std::vector<uint8>& subject_public_key_info);

#if defined(USE_NSS)
  
  
  
  
  
  static ECPrivateKey* CreateSensitiveFromEncryptedPrivateKeyInfo(
      PK11SlotInfo* slot,
      const std::string& password,
      const std::vector<uint8>& encrypted_private_key_info,
      const std::vector<uint8>& subject_public_key_info);
#endif

#if !defined(USE_OPENSSL)
  
  
  
  
  static bool ImportFromEncryptedPrivateKeyInfo(
      PK11SlotInfo* slot,
      const std::string& password,
      const uint8* encrypted_private_key_info,
      size_t encrypted_private_key_info_len,
      CERTSubjectPublicKeyInfo* decoded_spki,
      bool permanent,
      bool sensitive,
      SECKEYPrivateKey** key,
      SECKEYPublicKey** public_key);
#endif

#if defined(USE_OPENSSL)
  EVP_PKEY* key() { return key_; }
#else
  SECKEYPrivateKey* key() { return key_; }
  SECKEYPublicKey* public_key() { return public_key_; }
#endif

  
  
  
  
  
  bool ExportEncryptedPrivateKey(const std::string& password,
                                 int iterations,
                                 std::vector<uint8>* output);

  
  bool ExportPublicKey(std::vector<uint8>* output);

  
  
  bool ExportValue(std::vector<uint8>* output);
  bool ExportECParams(std::vector<uint8>* output);

 private:
  
  ECPrivateKey();

#if !defined(USE_OPENSSL)
  
  
  
  static ECPrivateKey* CreateWithParams(PK11SlotInfo* slot,
                                        bool permanent,
                                        bool sensitive);

  
  
  static ECPrivateKey* CreateFromEncryptedPrivateKeyInfoWithParams(
      PK11SlotInfo* slot,
      const std::string& password,
      const std::vector<uint8>& encrypted_private_key_info,
      const std::vector<uint8>& subject_public_key_info,
      bool permanent,
      bool sensitive);
#endif

#if defined(USE_OPENSSL)
  EVP_PKEY* key_;
#else
  SECKEYPrivateKey* key_;
  SECKEYPublicKey* public_key_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ECPrivateKey);
};


}  

#endif  
