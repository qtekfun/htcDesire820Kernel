// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_RSA_PRIVATE_KEY_H_
#define CRYPTO_RSA_PRIVATE_KEY_H_
#pragma once

#include "build/build_config.h"

#if defined(USE_OPENSSL)
typedef struct evp_pkey_st EVP_PKEY;
#elif defined(USE_NSS)
struct SECKEYPrivateKeyStr;
struct SECKEYPublicKeyStr;
#elif defined(OS_MACOSX)
#include <Security/cssm.h>
#endif

#include <list>
#include <vector>

#include "base/basictypes.h"

#if defined(OS_WIN)
#include "crypto/scoped_capi_types.h"
#endif
#if defined(USE_NSS)
#include "base/gtest_prod_util.h"
#endif

namespace crypto {

class PrivateKeyInfoCodec {
 public:

  
  static const uint8 kRsaAlgorithmIdentifier[];

  
  static const uint8 kBitStringTag = 0x03;
  static const uint8 kIntegerTag = 0x02;
  static const uint8 kNullTag = 0x05;
  static const uint8 kOctetStringTag = 0x04;
  static const uint8 kSequenceTag = 0x30;

  
  
  
  
  explicit PrivateKeyInfoCodec(bool big_endian);

  ~PrivateKeyInfoCodec();

  
  
  bool Export(std::vector<uint8>* output);

  
  
  bool ExportPublicKeyInfo(std::vector<uint8>* output);

  
  
  bool ExportPublicKey(std::vector<uint8>* output);

  
  
  
  
  bool Import(const std::vector<uint8>& input);

  
  
  std::vector<uint8>* modulus() { return &modulus_; };
  std::vector<uint8>* public_exponent() { return &public_exponent_; };
  std::vector<uint8>* private_exponent() { return &private_exponent_; };
  std::vector<uint8>* prime1() { return &prime1_; };
  std::vector<uint8>* prime2() { return &prime2_; };
  std::vector<uint8>* exponent1() { return &exponent1_; };
  std::vector<uint8>* exponent2() { return &exponent2_; };
  std::vector<uint8>* coefficient() { return &coefficient_; };

 private:
  
  
  void PrependInteger(const std::vector<uint8>& in, std::list<uint8>* out);
  void PrependInteger(uint8* val, int num_bytes, std::list<uint8>* data);

  
  
  void PrependIntegerImpl(uint8* val,
                          int num_bytes,
                          std::list<uint8>* data,
                          bool big_endian);

  
  
  bool ReadInteger(uint8** pos, uint8* end, std::vector<uint8>* out);
  bool ReadIntegerWithExpectedSize(uint8** pos,
                                   uint8* end,
                                   size_t expected_size,
                                   std::vector<uint8>* out);

  
  
  bool ReadIntegerImpl(uint8** pos,
                       uint8* end,
                       std::vector<uint8>* out,
                       bool big_endian);

  
  
  void PrependBytes(uint8* val,
                    int start,
                    int num_bytes,
                    std::list<uint8>* data);

  
  void PrependLength(size_t size, std::list<uint8>* data);

  
  void PrependTypeHeaderAndLength(uint8 type,
                                  uint32 length,
                                  std::list<uint8>* output);

  
  void PrependBitString(uint8* val, int num_bytes, std::list<uint8>* output);

  
  
  bool ReadLength(uint8** pos, uint8* end, uint32* result);

  
  bool ReadTypeHeaderAndLength(uint8** pos,
                               uint8* end,
                               uint8 expected_tag,
                               uint32* length);

  
  
  bool ReadSequence(uint8** pos, uint8* end);

  
  bool ReadAlgorithmIdentifier(uint8** pos, uint8* end);

  
  bool ReadVersion(uint8** pos, uint8* end);

  
  bool big_endian_;

  
  std::vector<uint8> modulus_;
  std::vector<uint8> public_exponent_;
  std::vector<uint8> private_exponent_;
  std::vector<uint8> prime1_;
  std::vector<uint8> prime2_;
  std::vector<uint8> exponent1_;
  std::vector<uint8> exponent2_;
  std::vector<uint8> coefficient_;

  DISALLOW_COPY_AND_ASSIGN(PrivateKeyInfoCodec);
};

class RSAPrivateKey {
 public:
  ~RSAPrivateKey();

  
  static RSAPrivateKey* Create(uint16 num_bits);

  
  
  
  
  static RSAPrivateKey* CreateSensitive(uint16 num_bits);

  
  
  
  static RSAPrivateKey* CreateFromPrivateKeyInfo(
      const std::vector<uint8>& input);

  
  
  
  
  
  
  static RSAPrivateKey* CreateSensitiveFromPrivateKeyInfo(
      const std::vector<uint8>& input);

  
  
  
  
  
  
  
  
  static RSAPrivateKey* FindFromPublicKeyInfo(
      const std::vector<uint8>& input);

#if defined(USE_OPENSSL)
  EVP_PKEY* key() { return key_; }
#elif defined(USE_NSS)
  SECKEYPrivateKeyStr* key() { return key_; }
  SECKEYPublicKeyStr* public_key() { return public_key_; }
#elif defined(OS_WIN)
  HCRYPTPROV provider() { return provider_; }
  HCRYPTKEY key() { return key_; }
#elif defined(OS_MACOSX)
  CSSM_KEY_PTR key() { return &key_; }
  CSSM_KEY_PTR public_key() { return &public_key_; }
#endif

  
  bool ExportPrivateKey(std::vector<uint8>* output);

  
  bool ExportPublicKey(std::vector<uint8>* output);

 private:
#if defined(USE_NSS)
  FRIEND_TEST_ALL_PREFIXES(RSAPrivateKeyNSSTest, FindFromPublicKey);
  FRIEND_TEST_ALL_PREFIXES(RSAPrivateKeyNSSTest, FailedFindFromPublicKey);
#endif

  
  
  RSAPrivateKey();

  
  
  
  static RSAPrivateKey* CreateWithParams(uint16 num_bits,
                                         bool permanent,
                                         bool sensitive);

  
  
  static RSAPrivateKey* CreateFromPrivateKeyInfoWithParams(
      const std::vector<uint8>& input, bool permanent, bool sensitive);

#if defined(USE_OPENSSL)
  EVP_PKEY* key_;
#elif defined(USE_NSS)
  SECKEYPrivateKeyStr* key_;
  SECKEYPublicKeyStr* public_key_;
#elif defined(OS_WIN)
  bool InitProvider();

  ScopedHCRYPTPROV provider_;
  ScopedHCRYPTKEY key_;
#elif defined(OS_MACOSX)
  CSSM_KEY key_;
  CSSM_KEY public_key_;
#endif

  DISALLOW_COPY_AND_ASSIGN(RSAPrivateKey);
};

}  

#endif  
