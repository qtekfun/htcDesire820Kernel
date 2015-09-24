// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_STRING_ESCAPE_H_
#define BASE_JSON_STRING_ESCAPE_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/string16.h"

namespace base {

BASE_API void JsonDoubleQuote(const std::string& str,
                              bool put_in_quotes,
                              std::string* dst);

BASE_API std::string GetDoubleQuotedJson(const std::string& str);

BASE_API void JsonDoubleQuote(const string16& str,
                              bool put_in_quotes,
                              std::string* dst);

BASE_API std::string GetDoubleQuotedJson(const string16& str);

}  

#endif  
