// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_SYS_STRING_CONVERSIONS_H_
#define BASE_STRINGS_SYS_STRING_CONVERSIONS_H_


#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/strings/string_piece.h"

#if defined(OS_MACOSX)
#include <CoreFoundation/CoreFoundation.h>
#ifdef __OBJC__
@class NSString;
#else
class NSString;
#endif
#endif  

namespace base {

BASE_EXPORT std::string SysWideToUTF8(const std::wstring& wide);
BASE_EXPORT std::wstring SysUTF8ToWide(const StringPiece& utf8);

BASE_EXPORT std::string SysWideToNativeMB(const std::wstring& wide);
BASE_EXPORT std::wstring SysNativeMBToWide(const StringPiece& native_mb);


#if defined(OS_WIN)

BASE_EXPORT std::wstring SysMultiByteToWide(const StringPiece& mb,
                                            uint32 code_page);
BASE_EXPORT std::string SysWideToMultiByte(const std::wstring& wide,
                                           uint32 code_page);

#endif  


#if defined(OS_MACOSX)


BASE_EXPORT CFStringRef SysUTF8ToCFStringRef(const std::string& utf8);
BASE_EXPORT CFStringRef SysUTF16ToCFStringRef(const string16& utf16);

BASE_EXPORT NSString* SysUTF8ToNSString(const std::string& utf8);
BASE_EXPORT NSString* SysUTF16ToNSString(const string16& utf16);

BASE_EXPORT std::string SysCFStringRefToUTF8(CFStringRef ref);
BASE_EXPORT string16 SysCFStringRefToUTF16(CFStringRef ref);

BASE_EXPORT std::string SysNSStringToUTF8(NSString* ref);
BASE_EXPORT string16 SysNSStringToUTF16(NSString* ref);

#endif  

}  

#endif  
