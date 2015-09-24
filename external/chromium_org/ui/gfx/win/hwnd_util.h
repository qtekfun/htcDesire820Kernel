// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_WIN_HWND_UTIL_H_
#define UI_GFX_WIN_HWND_UTIL_H_

#include <windows.h>

#include "base/strings/string16.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {
class Point;
class Size;

GFX_EXPORT string16 GetClassName(HWND hwnd);

GFX_EXPORT WNDPROC SetWindowProc(HWND hwnd, WNDPROC wndproc);

GFX_EXPORT void* SetWindowUserData(HWND hwnd, void* user_data);
GFX_EXPORT void* GetWindowUserData(HWND hwnd);

GFX_EXPORT bool DoesWindowBelongToActiveWindow(HWND window);

GFX_EXPORT void CenterAndSizeWindow(HWND parent,
                                    HWND window,
                                    const gfx::Size& pref);

GFX_EXPORT void CheckWindowCreated(HWND hwnd);

GFX_EXPORT void ShowSystemMenu(HWND window);
GFX_EXPORT void ShowSystemMenuAtPoint(HWND window, const gfx::Point& point);

GFX_EXPORT HWND GetWindowToParentTo(bool get_real_hwnd);

}  

#endif  
