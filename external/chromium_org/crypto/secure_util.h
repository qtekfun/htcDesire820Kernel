// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SECURE_UTIL_H_
#define CRYPTO_SECURE_UTIL_H_

#include <stddef.h>

#include "crypto/crypto_export.h"

namespace crypto {

CRYPTO_EXPORT bool SecureMemEqual(const void* s1, const void* s2, size_t n);

}  

#endif  

