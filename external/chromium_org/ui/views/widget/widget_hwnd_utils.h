// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_WIDGET_HWND_UTILS_H_
#define UI_VIEWS_WIDGET_WIDGET_HWND_UTILS_H_

#include <windows.h>

#include "ui/views/widget/widget.h"


namespace views {
class HWNDMessageHandler;
class WidgetDelegate;
namespace internal {
class NativeWidgetDelegate;
}

bool DidClientAreaSizeChange(const WINDOWPOS* window_pos);

void ConfigureWindowStyles(
    HWNDMessageHandler* handler,
    const Widget::InitParams& params,
    WidgetDelegate* widget_delegate,
    internal::NativeWidgetDelegate* native_widget_delegate);

}  

#endif  
