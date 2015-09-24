// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CRYPTO_HMAC_H_
#define CRYPTO_HMAC_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "crypto/crypto_export.h"

namespace crypto {

struct HMACPlatformData;
class SymmetricKey;

class CRYPTO_EXPORT HMAC {
 public:
  
  enum HashAlgorithm {
    SHA1,
    SHA256,
  };

  explicit HMAC(HashAlgorithm hash_alg);
  ~HMAC();

  
  size_t DigestLength() const;

  

  
  
  
  
  
  
  
  
  
  
  bool Init(const unsigned char* key, size_t key_length) WARN_UNUSED_RESULT;

  
  
  bool Init(SymmetricKey* key) WARN_UNUSED_RESULT;

  
  
  bool Init(const base::StringPiece& key) WARN_UNUSED_RESULT {
    return Init(reinterpret_cast<const unsigned char*>(key.data()),
                key.size());
  }

  
  
  
  bool Sign(const base::StringPiece& data, unsigned char* digest,
            size_t digest_length) const WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  bool Verify(const base::StringPiece& data,
              const base::StringPiece& digest) const WARN_UNUSED_RESULT;

  
  
  bool VerifyTruncated(
      const base::StringPiece& data,
      const base::StringPiece& digest) const WARN_UNUSED_RESULT;

 private:
  HashAlgorithm hash_alg_;
  scoped_ptr<HMACPlatformData> plat_;

  DISALLOW_COPY_AND_ASSIGN(HMAC);
};

}  

#endif  
