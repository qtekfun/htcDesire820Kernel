// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_DEBUG_DEBUG_ON_START_WIN_H_
#define BASE_DEBUG_DEBUG_ON_START_WIN_H_
#pragma once

#include "base/basictypes.h"
#include "build/build_config.h"

#if defined(OS_WIN)

#ifndef DECLSPEC_SELECTANY
#define DECLSPEC_SELECTANY  __declspec(selectany)
#endif

namespace base {
namespace debug {

#if !defined(BASE_DLL)

class DebugOnStart {
 public:
  
  
  typedef int  (__cdecl *PIFV)(void);

  
  
  static int __cdecl Init();

  
  
  static bool FindArgument(wchar_t* command_line, const char* argument);
};

#ifdef _WIN64

#pragma const_seg(push, ".CRT$XIB")
extern const DebugOnStart::PIFV debug_on_start;
DECLSPEC_SELECTANY const DebugOnStart::PIFV debug_on_start =
    &DebugOnStart::Init;
#pragma const_seg(pop)

#else  

#pragma data_seg(push, ".CRT$XIB")
DECLSPEC_SELECTANY DebugOnStart::PIFV debug_on_start = &DebugOnStart::Init;
#pragma data_seg(pop)

#endif  

#endif  

}  
}  

#endif  

#endif  
