// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_NATIVE_LIBRARY_H_
#define BASE_NATIVE_LIBRARY_H_
#pragma once


#include "base/base_api.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_MACOSX)
#import <CoreFoundation/CoreFoundation.h>
#endif  

#include "base/string16.h"

#if defined(OS_WIN) && !defined(CDECL)
#define CDECL __cdecl
#else
#define CDECL
#endif

class FilePath;

namespace base {

#if defined(OS_WIN)
typedef HMODULE NativeLibrary;
#elif defined(OS_MACOSX)
enum NativeLibraryType {
  BUNDLE,
  DYNAMIC_LIB
};
struct NativeLibraryStruct {
  NativeLibraryType type;
  CFBundleRefNum bundle_resource_ref;
  union {
    CFBundleRef bundle;
    void* dylib;
  };
};
typedef NativeLibraryStruct* NativeLibrary;
#elif defined(OS_POSIX)
typedef void* NativeLibrary;
#endif  

BASE_API NativeLibrary LoadNativeLibrary(const FilePath& library_path,
                                         std::string* error);

#if defined(OS_WIN)
BASE_API NativeLibrary LoadNativeLibraryDynamically(
    const FilePath& library_path);
#endif  

BASE_API void UnloadNativeLibrary(NativeLibrary library);

BASE_API void* GetFunctionPointerFromNativeLibrary(NativeLibrary library,
                                                   const char* name);

BASE_API string16 GetNativeLibraryName(const string16& name);

}  

#endif  
