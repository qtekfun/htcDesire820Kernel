// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_TRANSLATE_COMMON_TRANSLATE_UTIL_H_
#define COMPONENTS_TRANSLATE_COMMON_TRANSLATE_UTIL_H_

#include <string>

class GURL;

namespace translate {

extern const char kSecurityOrigin[];

void ToTranslateLanguageSynonym(std::string* language);

void ToChromeLanguageSynonym(std::string* language);

GURL GetTranslateSecurityOrigin();

}  

#endif  
