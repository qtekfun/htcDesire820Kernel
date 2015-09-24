// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SHA1_H_
#define BASE_SHA1_H_
#pragma once

#include <string>

#include "base/base_api.h"

namespace base {


enum {
  SHA1_LENGTH = 20  
};

BASE_API std::string SHA1HashString(const std::string& str);

BASE_API void SHA1HashBytes(const unsigned char* data, size_t len,
                            unsigned char* hash);

}  

#endif  
