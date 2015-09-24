// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_MOUSE_WHEEL_UTIL_H_
#define UI_BASE_WIN_MOUSE_WHEEL_UTIL_H_

#include <windows.h>

#include "ui/base/ui_export.h"

namespace ui {

class ViewProp;

UI_EXPORT ViewProp* SetWindowSupportsRerouteMouseWheel(HWND hwnd);

UI_EXPORT bool RerouteMouseWheel(HWND window, WPARAM w_param,
                                 LPARAM l_param);

}  

#endif  
