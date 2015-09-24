// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_UTF_STRING_CONVERSIONS_H_
#define BASE_UTF_STRING_CONVERSIONS_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/string16.h"
#include "base/string_piece.h"

BASE_API bool WideToUTF8(const wchar_t* src, size_t src_len,
                         std::string* output);
BASE_API std::string WideToUTF8(const std::wstring& wide);
BASE_API bool UTF8ToWide(const char* src, size_t src_len, std::wstring* output);
BASE_API std::wstring UTF8ToWide(const base::StringPiece& utf8);

BASE_API bool WideToUTF16(const wchar_t* src, size_t src_len, string16* output);
BASE_API string16 WideToUTF16(const std::wstring& wide);
BASE_API bool UTF16ToWide(const char16* src, size_t src_len,
                          std::wstring* output);
BASE_API std::wstring UTF16ToWide(const string16& utf16);

BASE_API bool UTF8ToUTF16(const char* src, size_t src_len, string16* output);
BASE_API string16 UTF8ToUTF16(const base::StringPiece& utf8);
BASE_API bool UTF16ToUTF8(const char16* src, size_t src_len,
                          std::string* output);
BASE_API std::string UTF16ToUTF8(const string16& utf16);

#if defined(OS_WIN)
# define WideToUTF16Hack
# define UTF16ToWideHack
#else
# define WideToUTF16Hack WideToUTF16
# define UTF16ToWideHack UTF16ToWide
#endif

BASE_API std::wstring ASCIIToWide(const base::StringPiece& ascii);
BASE_API string16 ASCIIToUTF16(const base::StringPiece& ascii);

#endif  
