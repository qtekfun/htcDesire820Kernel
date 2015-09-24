// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SHA2_H_
#define CRYPTO_SHA2_H_

#include <string>

#include "base/strings/string_piece.h"
#include "crypto/crypto_export.h"

namespace crypto {


static const size_t kSHA256Length = 32;  

CRYPTO_EXPORT void SHA256HashString(const base::StringPiece& str,
                                    void* output, size_t len);

CRYPTO_EXPORT std::string SHA256HashString(const base::StringPiece& str);

}  

#endif  
