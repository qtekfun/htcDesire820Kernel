// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SECURE_HASH_H_
#define CRYPTO_SECURE_HASH_H_

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

class Pickle;
class PickleIterator;

namespace crypto {

class CRYPTO_EXPORT SecureHash {
 public:
  enum Algorithm {
    SHA256,
  };
  virtual ~SecureHash() {}

  static SecureHash* Create(Algorithm type);

  virtual void Update(const void* input, size_t len) = 0;
  virtual void Finish(void* output, size_t len) = 0;

  
  
  
  virtual bool Serialize(Pickle* pickle) = 0;

  
  
  
  
  virtual bool Deserialize(PickleIterator* data_iterator) = 0;

 protected:
  SecureHash() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(SecureHash);
};

}  

#endif  
