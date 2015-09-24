// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_ENCRYPTOR_H_
#define CRYPTO_ENCRYPTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "build/build_config.h"
#include "crypto/crypto_export.h"

#if defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
#include "crypto/scoped_nss_types.h"
#endif

namespace crypto {

class SymmetricKey;

class CRYPTO_EXPORT Encryptor {
 public:
  enum Mode {
    CBC,
    CTR,
  };

  
  
  class CRYPTO_EXPORT Counter {
   public:
    explicit Counter(const base::StringPiece& counter);
    ~Counter();

    
    bool Increment();

    
    
    void Write(void* buf);

    
    size_t GetLengthInBytes() const;

   private:
    union {
      uint32 components32[4];
      uint64 components64[2];
    } counter_;
  };

  Encryptor();
  virtual ~Encryptor();

  
  
  
  
  
  bool Init(SymmetricKey* key, Mode mode, const base::StringPiece& iv);

  
  
  bool Encrypt(const base::StringPiece& plaintext, std::string* ciphertext);

  
  
  
  
  
  
  
  
  
  bool Decrypt(const base::StringPiece& ciphertext, std::string* plaintext);

  
  
  
  
  bool SetCounter(const base::StringPiece& counter);

  

 private:
  
  // Resulting mask will be written to |mask| with |mask_len| bytes.
  
  
  
  
  
  
  
  
  
  
  bool GenerateCounterMask(size_t plaintext_len,
                           uint8* mask,
                           size_t* mask_len);

  // Mask the |plaintext| message using |mask|. The output will be written to
  
  void MaskMessage(const void* plaintext,
                   size_t plaintext_len,
                   const void* mask,
                   void* ciphertext) const;

  SymmetricKey* key_;
  Mode mode_;
  scoped_ptr<Counter> counter_;

#if defined(USE_OPENSSL)
  bool Crypt(bool do_encrypt,  
             const base::StringPiece& input,
             std::string* output);
  bool CryptCTR(bool do_encrypt,
                const base::StringPiece& input,
                std::string* output);
  std::string iv_;
#elif defined(USE_NSS) || defined(OS_WIN) || defined(OS_MACOSX)
  bool Crypt(PK11Context* context,
             const base::StringPiece& input,
             std::string* output);
  bool CryptCTR(PK11Context* context,
                const base::StringPiece& input,
                std::string* output);
  ScopedSECItem param_;
#endif
};

}  

#endif  
