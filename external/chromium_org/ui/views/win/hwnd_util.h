// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIN_HWND_UTIL_H_
#define UI_VIEWS_WIN_HWND_UTIL_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/views/views_export.h"

namespace views {

class View;
class Widget;

VIEWS_EXPORT HWND HWNDForView(const View* view);

VIEWS_EXPORT HWND HWNDForWidget(const Widget* widget);

VIEWS_EXPORT HWND HWNDForNativeView(const gfx::NativeView view);

VIEWS_EXPORT HWND HWNDForNativeWindow(const gfx::NativeWindow window);

VIEWS_EXPORT gfx::Rect GetWindowBoundsForClientBounds(
    View* view, const gfx::Rect& client_bounds);
}

#endif  
