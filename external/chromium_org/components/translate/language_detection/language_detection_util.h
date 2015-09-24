// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_TRANSLATE_LANGUAGE_DETECTION_LANGUAGE_DETECTION_UTIL_H_
#define COMPONENTS_TRANSLATE_LANGUAGE_DETECTION_LANGUAGE_DETECTION_UTIL_H_

#include <string>

#include "base/strings/string16.h"

namespace translate {

std::string DeterminePageLanguage(const std::string& code,
                                  const std::string& html_lang,
                                  const base::string16& contents,
                                  std::string* cld_language,
                                  bool* is_cld_reliable);

void CorrectLanguageCodeTypo(std::string* code);

bool IsValidLanguageCode(const std::string& code);

bool IsSameOrSimilarLanguages(const std::string& page_language,
                              const std::string& cld_language);

bool MaybeServerWrongConfiguration(const std::string& page_language,
                                   const std::string& cld_language);

}  

#endif  
