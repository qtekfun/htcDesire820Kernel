// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_MONITOR_WIN_H_
#define UI_VIEWS_WIDGET_MONITOR_WIN_H_

#include <windows.h>

#include "ui/views/views_export.h"

namespace gfx {
class Rect;
}

namespace views {

VIEWS_EXPORT gfx::Rect GetMonitorBoundsForRect(const gfx::Rect& rect);

VIEWS_EXPORT HWND GetTopmostAutoHideTaskbarForEdge(UINT edge, HMONITOR monitor);

}  

#endif  
