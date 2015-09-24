// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CRYPTO_HMAC_H_
#define CRYPTO_HMAC_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace crypto {

struct HMACPlatformData;

class HMAC {
 public:
  
  enum HashAlgorithm {
    SHA1,
    SHA256,
  };

  explicit HMAC(HashAlgorithm hash_alg);
  ~HMAC();

  
  
  bool Init(const unsigned char* key, int key_length);

  
  
  bool Init(const std::string& key) {
    return Init(reinterpret_cast<const unsigned char*>(key.data()),
                static_cast<int>(key.size()));
  }

  
  
  
  bool Sign(const std::string& data, unsigned char* digest, int digest_length);

  

 private:
  HashAlgorithm hash_alg_;
  scoped_ptr<HMACPlatformData> plat_;

  DISALLOW_COPY_AND_ASSIGN(HMAC);
};

}  

#endif  
