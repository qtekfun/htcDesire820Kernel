// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_LATIN1_STRING_CONVERSIONS_H_
#define BASE_STRINGS_LATIN1_STRING_CONVERSIONS_H_

#include <string>

#include "base/base_export.h"
#include "base/strings/string16.h"

namespace base {

typedef unsigned char Latin1Char;

BASE_EXPORT string16 Latin1OrUTF16ToUTF16(size_t length,
                                          const Latin1Char* latin1,
                                          const char16* utf16);

}  

#endif  
