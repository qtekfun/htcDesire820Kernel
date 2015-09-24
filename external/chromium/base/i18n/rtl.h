// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_RTL_H_
#define BASE_I18N_RTL_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"
#include "base/string16.h"
#include "build/build_config.h"

class FilePath;

namespace base {
namespace i18n {

const char16 kRightToLeftMark = 0x200F;
const char16 kLeftToRightMark = 0x200E;
const char16 kLeftToRightEmbeddingMark = 0x202A;
const char16 kRightToLeftEmbeddingMark = 0x202B;
const char16 kPopDirectionalFormatting = 0x202C;
const char16 kLeftToRightOverride = 0x202D;
const char16 kRightToLeftOverride = 0x202E;

enum TextDirection {
  UNKNOWN_DIRECTION,
  RIGHT_TO_LEFT,
  LEFT_TO_RIGHT,
};

std::string GetConfiguredLocale();

std::string GetCanonicalLocale(const char* locale);

void SetICUDefaultLocale(const std::string& locale_string);

bool IsRTL();

bool ICUIsRTL();

TextDirection GetTextDirectionForLocale(const char* locale_name);

TextDirection GetFirstStrongCharacterDirection(const string16& text);
#if defined(WCHAR_T_IS_UTF32)
TextDirection GetFirstStrongCharacterDirection(const std::wstring& text);
#endif

bool AdjustStringForLocaleDirection(string16* text);
#if defined(WCHAR_T_IS_UTF32)
bool AdjustStringForLocaleDirection(std::wstring* text);
#endif

bool StringContainsStrongRTLChars(const string16& text);
#if defined(WCHAR_T_IS_UTF32)
bool StringContainsStrongRTLChars(const std::wstring& text);
#endif

void WrapStringWithLTRFormatting(string16* text);
#if defined(WCHAR_T_IS_UTF32)
void WrapStringWithLTRFormatting(std::wstring* text);
#endif

void WrapStringWithRTLFormatting(string16* text);
#if defined(WCHAR_T_IS_UTF32)
void WrapStringWithRTLFormatting(std::wstring* text);
#endif

void WrapPathWithLTRFormatting(const FilePath& path,
                               string16* rtl_safe_path);

string16 GetDisplayStringInLTRDirectionality(const string16& text)
    WARN_UNUSED_RESULT;

const string16 StripWrappingBidiControlCharacters(const string16& text)
    WARN_UNUSED_RESULT;

}  
}  

#endif  
