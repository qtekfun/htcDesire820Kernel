// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_ICU_STRING_CONVERSIONS_H_
#define BASE_I18N_ICU_STRING_CONVERSIONS_H_
#pragma once

#include <string>

#include "base/string16.h"

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

extern const char kCodepageLatin1[];  
extern const char kCodepageUTF8[];
extern const char kCodepageUTF16BE[];
extern const char kCodepageUTF16LE[];

bool UTF16ToCodepage(const string16& utf16,
                     const char* codepage_name,
                     OnStringConversionError::Type on_error,
                     std::string* encoded);
bool CodepageToUTF16(const std::string& encoded,
                     const char* codepage_name,
                     OnStringConversionError::Type on_error,
                     string16* utf16);

bool WideToCodepage(const std::wstring& wide,
                    const char* codepage_name,
                    OnStringConversionError::Type on_error,
                    std::string* encoded);
bool CodepageToWide(const std::string& encoded,
                    const char* codepage_name,
                    OnStringConversionError::Type on_error,
                    std::wstring* wide);

bool ConvertToUtf8AndNormalize(const std::string& text,
                               const std::string& charset,
                               std::string* result);

}  

#endif  
