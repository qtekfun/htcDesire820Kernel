// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_WIN_WRAPPED_WINDOW_PROC_H_
#define BASE_WIN_WRAPPED_WINDOW_PROC_H_
#pragma once

#include <windows.h>

#include "base/base_api.h"

namespace base {
namespace win {

typedef int (__cdecl *WinProcExceptionFilter)(EXCEPTION_POINTERS* info);

BASE_API WinProcExceptionFilter SetWinProcExceptionFilter(
    WinProcExceptionFilter filter);

BASE_API int CallExceptionFilter(EXCEPTION_POINTERS* info);

template <WNDPROC proc>
LRESULT CALLBACK WrappedWindowProc(HWND hwnd, UINT message,
                                   WPARAM wparam, LPARAM lparam) {
  LRESULT rv = 0;
  __try {
    rv = proc(hwnd, message, wparam, lparam);
  } __except(CallExceptionFilter(GetExceptionInformation())) {
  }
  return rv;
}

}  
}  

#endif  
