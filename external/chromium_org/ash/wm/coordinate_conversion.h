// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_COORDINATE_CONVERSION_H_
#define ASH_WM_COORDINATE_CONVERSION_H_

#include "ash/ash_export.h"

namespace aura {
class Window;
}  

namespace gfx {
class Point;
class Rect;
}  

namespace ash {
namespace wm {

ASH_EXPORT aura::Window* GetRootWindowAt(const gfx::Point& point);

ASH_EXPORT aura::Window* GetRootWindowMatching(const gfx::Rect& rect);

ASH_EXPORT void ConvertPointToScreen(aura::Window* window, gfx::Point* point);

ASH_EXPORT void ConvertPointFromScreen(aura::Window* window,
                                       gfx::Point* point_in_screen);

}  
}  

#endif  
