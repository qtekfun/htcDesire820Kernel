// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_JSON_STRING_ESCAPE_H_
#define BASE_JSON_STRING_ESCAPE_H_

#include <string>

#include "base/base_export.h"
#include "base/strings/string_piece.h"

namespace base {

BASE_EXPORT bool EscapeJSONString(const StringPiece& str,
                                  bool put_in_quotes,
                                  std::string* dest);

BASE_EXPORT bool EscapeJSONString(const StringPiece16& str,
                                  bool put_in_quotes,
                                  std::string* dest);

BASE_EXPORT std::string GetQuotedJSONString(const StringPiece& str);
BASE_EXPORT std::string GetQuotedJSONString(const StringPiece16& str);

BASE_EXPORT std::string EscapeBytesAsInvalidJSONString(const StringPiece& str,
                                                       bool put_in_quotes);

}  

#endif  
