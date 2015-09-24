// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MD5_H_
#define BASE_MD5_H_
#pragma once

#include <string>

#include "base/base_api.h"



typedef struct MD5Digest_struct {
  unsigned char a[16];
} MD5Digest;

typedef char MD5Context[88];

BASE_API void MD5Sum(const void* data, size_t length, MD5Digest* digest);

BASE_API void MD5Init(MD5Context* context);

BASE_API void MD5Update(MD5Context* context, const void* buf, size_t len);

BASE_API void MD5Final(MD5Digest* digest, MD5Context* pCtx);

BASE_API std::string MD5DigestToBase16(const MD5Digest& digest);

BASE_API std::string MD5String(const std::string& str);

#endif  

