// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_EC_SIGNATURE_CREATOR_H_
#define CRYPTO_EC_SIGNATURE_CREATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

namespace crypto {

class ECPrivateKey;
class ECSignatureCreator;

class CRYPTO_EXPORT ECSignatureCreatorFactory {
 public:
  virtual ~ECSignatureCreatorFactory() {}

  virtual ECSignatureCreator* Create(ECPrivateKey* key) = 0;
};

class CRYPTO_EXPORT ECSignatureCreator {
 public:
  virtual ~ECSignatureCreator() {}

  
  
  
  
  static ECSignatureCreator* Create(ECPrivateKey* key);

  
  
  
  
  static void SetFactoryForTesting(ECSignatureCreatorFactory* factory);

  
  
  
  
  
  
  virtual bool Sign(const uint8* data,
                    int data_len,
                    std::vector<uint8>* signature) = 0;

  
  
  
  
  
  virtual bool DecodeSignature(const std::vector<uint8>& signature,
                               std::vector<uint8>* out_raw_sig) = 0;
};

}  

#endif  
