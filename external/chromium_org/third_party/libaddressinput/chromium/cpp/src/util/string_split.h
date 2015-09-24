// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef I18N_ADDRESSINPUT_UTIL_STRING_SPLIT_H_
#define I18N_ADDRESSINPUT_UTIL_STRING_SPLIT_H_

#include <string>
#include <vector>

namespace i18n {
namespace addressinput {

void SplitString(const std::string& str, char c, std::vector<std::string>* r);

}  
}  

#endif  
