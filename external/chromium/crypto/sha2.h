// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SHA2_H_
#define CRYPTO_SHA2_H_
#pragma once

#include <string>

namespace crypto {


enum {
  SHA256_LENGTH = 32  
};

void SHA256HashString(const std::string& str,
                      void* output, size_t len);

std::string SHA256HashString(const std::string& str);

}  

#endif  
