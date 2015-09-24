// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MD5_H_
#define BASE_MD5_H_

#include "base/base_export.h"
#include "base/strings/string_piece.h"

namespace base {



struct MD5Digest {
  unsigned char a[16];
};

typedef char MD5Context[88];

BASE_EXPORT void MD5Sum(const void* data, size_t length, MD5Digest* digest);

BASE_EXPORT void MD5Init(MD5Context* context);

BASE_EXPORT void MD5Update(MD5Context* context, const StringPiece& data);

BASE_EXPORT void MD5Final(MD5Digest* digest, MD5Context* context);

BASE_EXPORT std::string MD5DigestToBase16(const MD5Digest& digest);

BASE_EXPORT std::string MD5String(const StringPiece& str);

}  

#endif  
