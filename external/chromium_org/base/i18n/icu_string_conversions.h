// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_ICU_STRING_CONVERSIONS_H_
#define BASE_I18N_ICU_STRING_CONVERSIONS_H_

#include <string>

#include "base/i18n/base_i18n_export.h"
#include "base/i18n/i18n_constants.h"
#include "base/strings/string16.h"

namespace base {

class OnStringConversionError {
 public:
  enum Type {
    
    FAIL,

    
    
    SKIP,

    
    
    
    SUBSTITUTE,
  };

 private:
  OnStringConversionError();
};

BASE_I18N_EXPORT bool UTF16ToCodepage(const string16& utf16,
                                      const char* codepage_name,
                                      OnStringConversionError::Type on_error,
                                      std::string* encoded);
BASE_I18N_EXPORT bool CodepageToUTF16(const std::string& encoded,
                                      const char* codepage_name,
                                      OnStringConversionError::Type on_error,
                                      string16* utf16);

BASE_I18N_EXPORT bool WideToCodepage(const std::wstring& wide,
                                     const char* codepage_name,
                                     OnStringConversionError::Type on_error,
                                     std::string* encoded);
BASE_I18N_EXPORT bool CodepageToWide(const std::string& encoded,
                                     const char* codepage_name,
                                     OnStringConversionError::Type on_error,
                                     std::wstring* wide);

BASE_I18N_EXPORT bool ConvertToUtf8AndNormalize(const std::string& text,
                                                const std::string& charset,
                                                std::string* result);

}  

#endif  
