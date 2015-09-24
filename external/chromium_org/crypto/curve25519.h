// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_CURVE25519_H
#define CRYPTO_CURVE25519_H

#include "base/basictypes.h"
#include "crypto/crypto_export.h"

namespace crypto {

namespace curve25519 {

static const size_t kBytes = 32;

static const size_t kScalarBytes = 32;

CRYPTO_EXPORT void ScalarMult(const uint8* private_key,
                              const uint8* peer_public_key,
                              uint8* shared_key);

CRYPTO_EXPORT void ScalarBaseMult(const uint8* private_key, uint8* public_key);

}  

}  

#endif  
