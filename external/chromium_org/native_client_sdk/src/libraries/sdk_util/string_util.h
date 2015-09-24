// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_SDK_UTIL_STRING_UTIL_H_
#define LIBRARIES_SDK_UTIL_STRING_UTIL_H_

#include <string>
#include <vector>

namespace sdk_util {

inline void SplitString(const std::string& str,
                        char c,
                        std::vector<std::string>* r) {
  r->clear();
  size_t last = 0;
  size_t size = str.size();
  for (size_t i = 0; i <= size; ++i) {
    if (i == size || str[i] == c) {
      std::string tmp(str, last, i - last);
      
      
      if (i != size || !r->empty() || !tmp.empty())
        r->push_back(tmp);
      last = i + 1;
    }
  }
}

}  

#endif  
