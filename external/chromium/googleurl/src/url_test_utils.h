// Copyright 2007 Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLEURL_SRC_URL_TEST_UTILS_H__
#define GOOGLEURL_SRC_URL_TEST_UTILS_H__

#include <string>

#include "base/string16.h"
#include "googleurl/src/url_canon_internal.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace url_test_utils {

inline string16 WStringToUTF16(const wchar_t* src) {
  string16 str;
  int length = static_cast<int>(wcslen(src));
  for (int i = 0; i < length; ++i) {
    str.push_back(static_cast<char16>(src[i]));
  }
  return str;
}

inline string16 ConvertUTF8ToUTF16(const std::string& src) {
  int length = static_cast<int>(src.length());
  EXPECT_LT(length, 1024);
  url_canon::RawCanonOutputW<1024> output;
  EXPECT_TRUE(url_canon::ConvertUTF8ToUTF16(src.data(), length, &output));
  return string16(output.data(), output.length());
}

inline std::string ConvertUTF16ToUTF8(const string16& src) {
  std::string str;
  url_canon::StdStringCanonOutput output(&str);
  EXPECT_TRUE(url_canon::ConvertUTF16ToUTF8(src.data(),
                                            static_cast<int>(src.length()),
                                            &output));
  output.Complete();
  return str;
}

}  

#endif  
