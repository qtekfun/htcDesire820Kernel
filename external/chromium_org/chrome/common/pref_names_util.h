// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_PREF_NAMES_UTIL_H_
#define CHROME_COMMON_PREF_NAMES_UTIL_H_

#include <string>

namespace pref_names_util {

bool ParseFontNamePrefPath(const std::string& pref_path,
                           std::string* generic_family,
                           std::string* script);

}  

#endif  
