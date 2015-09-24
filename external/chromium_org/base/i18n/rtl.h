// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_RTL_H_
#define BASE_I18N_RTL_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"
#include "build/build_config.h"

namespace base {

class FilePath;

namespace i18n {

const char16 kRightToLeftMark = 0x200F;
const char16 kLeftToRightMark = 0x200E;
const char16 kLeftToRightEmbeddingMark = 0x202A;
const char16 kRightToLeftEmbeddingMark = 0x202B;
const char16 kPopDirectionalFormatting = 0x202C;
const char16 kLeftToRightOverride = 0x202D;
const char16 kRightToLeftOverride = 0x202E;

enum TextDirection {
  UNKNOWN_DIRECTION = 0,
  RIGHT_TO_LEFT = 1,
  LEFT_TO_RIGHT = 2,
  TEXT_DIRECTION_NUM_DIRECTIONS = 3,
};

BASE_I18N_EXPORT std::string GetConfiguredLocale();

BASE_I18N_EXPORT std::string GetCanonicalLocale(const char* locale);

BASE_I18N_EXPORT void SetICUDefaultLocale(const std::string& locale_string);

BASE_I18N_EXPORT bool IsRTL();

BASE_I18N_EXPORT bool ICUIsRTL();

BASE_I18N_EXPORT TextDirection GetTextDirectionForLocale(
    const char* locale_name);

BASE_I18N_EXPORT TextDirection GetFirstStrongCharacterDirection(
    const string16& text);

BASE_I18N_EXPORT TextDirection GetStringDirection(const string16& text);

BASE_I18N_EXPORT bool AdjustStringForLocaleDirection(string16* text);

BASE_I18N_EXPORT bool UnadjustStringForLocaleDirection(string16* text);

BASE_I18N_EXPORT bool StringContainsStrongRTLChars(const string16& text);

BASE_I18N_EXPORT void WrapStringWithLTRFormatting(string16* text);

BASE_I18N_EXPORT void WrapStringWithRTLFormatting(string16* text);

BASE_I18N_EXPORT void WrapPathWithLTRFormatting(const FilePath& path,
                                                string16* rtl_safe_path);

BASE_I18N_EXPORT string16 GetDisplayStringInLTRDirectionality(
    const string16& text) WARN_UNUSED_RESULT;

BASE_I18N_EXPORT string16 StripWrappingBidiControlCharacters(
    const string16& text) WARN_UNUSED_RESULT;

}  
}  

#endif  
