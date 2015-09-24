// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_WIN_WRAPPED_WINDOW_PROC_H_
#define BASE_WIN_WRAPPED_WINDOW_PROC_H_

#include <windows.h>

#include "base/base_export.h"
#include "base/strings/string16.h"

namespace base {
namespace win {

typedef int (__cdecl *WinProcExceptionFilter)(EXCEPTION_POINTERS* info);

BASE_EXPORT WinProcExceptionFilter SetWinProcExceptionFilter(
    WinProcExceptionFilter filter);

BASE_EXPORT int CallExceptionFilter(EXCEPTION_POINTERS* info);

BASE_EXPORT void InitializeWindowClass(
    const char16* class_name,
    WNDPROC window_proc,
    UINT style,
    int class_extra,
    int window_extra,
    HCURSOR cursor,
    HBRUSH background,
    const char16* menu_name,
    HICON large_icon,
    HICON small_icon,
    WNDCLASSEX* class_out);

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
