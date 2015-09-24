// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_RANDOM_H_
#define CRYPTO_RANDOM_H_

#include <stddef.h>

#include "crypto/crypto_export.h"

namespace crypto {

CRYPTO_EXPORT void RandBytes(void *bytes, size_t length);

}

#endif
