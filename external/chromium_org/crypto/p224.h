// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_P224_H_
#define CRYPTO_P224_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "crypto/crypto_export.h"

namespace crypto {

namespace p224 {

typedef uint32 FieldElement[8];

struct CRYPTO_EXPORT Point {
  
  
  
  
  bool SetFromString(const base::StringPiece& in);

  
  std::string ToString() const;

  
  FieldElement x, y, z;
};

static const size_t kScalarBytes = 28;

void CRYPTO_EXPORT ScalarMult(const Point& in, const uint8* scalar, Point* out);

void CRYPTO_EXPORT ScalarBaseMult(const uint8* scalar, Point* out);

void CRYPTO_EXPORT Add(const Point& a, const Point& b, Point* out);

void CRYPTO_EXPORT Negate(const Point& a, Point* out);

}  

}  

#endif  
