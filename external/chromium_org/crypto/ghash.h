// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

namespace crypto {

class CRYPTO_EXPORT_PRIVATE GaloisHash {
 public:
  explicit GaloisHash(const uint8 key[16]);

  
  
  void Reset();

  
  
  // be written before any ciphertext is written.
  void UpdateAdditional(const uint8* data, size_t length);

  
  void UpdateCiphertext(const uint8* data, size_t length);

  
  
  void Finish(void* output, size_t len);

 private:
  enum State {
    kHashingAdditionalData,
    kHashingCiphertext,
    kComplete,
  };

  struct FieldElement {
    uint64 low, hi;
  };

  
  static FieldElement Add(const FieldElement& x, const FieldElement& y);
  
  static FieldElement Double(const FieldElement& x);
  
  
  static void MulAfterPrecomputation(const FieldElement* table,
                                     FieldElement* x);
  
  static void Mul16(FieldElement* x);

  
  void UpdateBlocks(const uint8* bytes, size_t num_blocks);
  
  
  
  void Update(const uint8* bytes, size_t length);

  FieldElement y_;
  State state_;
  size_t additional_bytes_;
  size_t ciphertext_bytes_;
  uint8 buf_[16];
  size_t buf_used_;
  FieldElement product_table_[16];
};

}  
