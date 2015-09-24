// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRING_SPLIT_H_
#define BASE_STRING_SPLIT_H_
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "base/base_api.h"
#include "base/string16.h"

namespace base {

BASE_API void SplitString(const std::wstring& str,
                          wchar_t c,
                          std::vector<std::wstring>* r);
BASE_API void SplitString(const string16& str,
                          char16 c,
                          std::vector<string16>* r);
BASE_API void SplitString(const std::string& str,
                          char c,
                          std::vector<std::string>* r);

BASE_API bool SplitStringIntoKeyValues(
    const std::string& line,
    char key_value_delimiter,
    std::string* key, std::vector<std::string>* values);

BASE_API bool SplitStringIntoKeyValuePairs(
    const std::string& line,
    char key_value_delimiter,
    char key_value_pair_delimiter,
    std::vector<std::pair<std::string, std::string> >* kv_pairs);

BASE_API void SplitStringUsingSubstr(const string16& str,
                                     const string16& s,
                                     std::vector<string16>* r);
BASE_API void SplitStringUsingSubstr(const std::string& str,
                                     const std::string& s,
                                     std::vector<std::string>* r);

BASE_API void SplitStringDontTrim(const string16& str,
                                  char16 c,
                                  std::vector<string16>* r);
BASE_API void SplitStringDontTrim(const std::string& str,
                                  char c,
                                  std::vector<std::string>* r);

BASE_API void SplitStringAlongWhitespace(const std::wstring& str,
                                         std::vector<std::wstring>* result);
BASE_API void SplitStringAlongWhitespace(const string16& str,
                                         std::vector<string16>* result);
BASE_API void SplitStringAlongWhitespace(const std::string& str,
                                         std::vector<std::string>* result);

}  

#endif  
