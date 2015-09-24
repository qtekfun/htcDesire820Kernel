// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGPRINTF_H_
#define BASE_STRINGPRINTF_H_

#include <stdarg.h>   

#include <string>

#include "base/base_api.h"
#include "base/compiler_specific.h"

namespace base {

BASE_API std::string StringPrintf(const char* format, ...) PRINTF_FORMAT(1, 2);
BASE_API std::wstring StringPrintf(const wchar_t* format, ...)
    WPRINTF_FORMAT(1, 2);

BASE_API std::string StringPrintV(const char* format, va_list ap)
    PRINTF_FORMAT(1, 0);

BASE_API const std::string& SStringPrintf(std::string* dst,
                                          const char* format, ...)
    PRINTF_FORMAT(2, 3);
BASE_API const std::wstring& SStringPrintf(std::wstring* dst,
                                           const wchar_t* format, ...)
    WPRINTF_FORMAT(2, 3);

BASE_API void StringAppendF(std::string* dst, const char* format, ...)
    PRINTF_FORMAT(2, 3);
BASE_API void StringAppendF(std::wstring* dst, const wchar_t* format, ...)
    WPRINTF_FORMAT(2, 3);

BASE_API void StringAppendV(std::string* dst, const char* format, va_list ap)
    PRINTF_FORMAT(2, 0);
BASE_API void StringAppendV(std::wstring* dst,
                            const wchar_t* format, va_list ap)
    WPRINTF_FORMAT(2, 0);

}  

using base::StringPrintf;

#endif  
