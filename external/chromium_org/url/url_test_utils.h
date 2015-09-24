// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_TEST_UTILS_H_
#define URL_URL_TEST_UTILS_H_


#include <string>

#include "base/strings/string16.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/url_canon_internal.h"

namespace url_test_utils {

inline base::string16 WStringToUTF16(const wchar_t* src) {
  base::string16 str;
  int length = static_cast<int>(wcslen(src));
  for (int i = 0; i < length; ++i) {
    str.push_back(static_cast<char16>(src[i]));
  }
  return str;
}

inline base::string16 ConvertUTF8ToUTF16(const std::string& src) {
  int length = static_cast<int>(src.length());
  EXPECT_LT(length, 1024);
  url_canon::RawCanonOutputW<1024> output;
  EXPECT_TRUE(url_canon::ConvertUTF8ToUTF16(src.data(), length, &output));
  return base::string16(output.data(), output.length());
}

inline std::string ConvertUTF16ToUTF8(const base::string16& src) {
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
