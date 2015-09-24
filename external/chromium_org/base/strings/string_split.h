// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRING_SPLIT_H_
#define BASE_STRINGS_STRING_SPLIT_H_

#include <string>
#include <utility>
#include <vector>

#include "base/base_export.h"
#include "base/strings/string16.h"

namespace base {

BASE_EXPORT void SplitString(const string16& str,
                             char16 c,
                             std::vector<string16>* r);
BASE_EXPORT void SplitString(const std::string& str,
                             char c,
                             std::vector<std::string>* r);

BASE_EXPORT bool SplitStringIntoKeyValues(const std::string& line,
                                          char key_value_delimiter,
                                          std::string* key,
                                          std::vector<std::string>* values);

typedef std::vector<std::pair<std::string, std::string> > StringPairs;;

BASE_EXPORT bool SplitStringIntoKeyValuePairs(
    const std::string& line,
    char key_value_delimiter,
    char key_value_pair_delimiter,
    StringPairs* key_value_pairs);

BASE_EXPORT void SplitStringUsingSubstr(const string16& str,
                                        const string16& s,
                                        std::vector<string16>* r);
BASE_EXPORT void SplitStringUsingSubstr(const std::string& str,
                                        const std::string& s,
                                        std::vector<std::string>* r);

BASE_EXPORT void SplitStringDontTrim(const string16& str,
                                     char16 c,
                                     std::vector<string16>* r);
BASE_EXPORT void SplitStringDontTrim(const std::string& str,
                                     char c,
                                     std::vector<std::string>* r);

BASE_EXPORT void SplitStringAlongWhitespace(const string16& str,
                                            std::vector<string16>* result);
BASE_EXPORT void SplitStringAlongWhitespace(const std::string& str,
                                            std::vector<std::string>* result);

}  

#endif  
