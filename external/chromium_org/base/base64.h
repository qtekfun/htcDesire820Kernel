// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BASE64_H__
#define BASE_BASE64_H__

#include <string>

#include "base/base_export.h"
#include "base/strings/string_piece.h"

namespace base {

BASE_EXPORT void Base64Encode(const StringPiece& input, std::string* output);

BASE_EXPORT bool Base64Decode(const StringPiece& input, std::string* output);

}  

#endif  
