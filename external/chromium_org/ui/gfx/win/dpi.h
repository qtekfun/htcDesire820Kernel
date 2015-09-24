// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_DPI_WIN_H_
#define UI_GFX_DPI_WIN_H_

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

namespace gfx {

GFX_EXPORT void InitDeviceScaleFactor(float scale);

GFX_EXPORT Size GetDPI();

GFX_EXPORT float GetDPIScale();

GFX_EXPORT float GetModernUIScale();

GFX_EXPORT bool IsHighDPIEnabled();

GFX_EXPORT bool IsInHighDPIMode();

GFX_EXPORT void EnableHighDPISupport();


namespace win {

GFX_EXPORT float GetDeviceScaleFactor();

GFX_EXPORT Point ScreenToDIPPoint(const Point& pixel_point);

GFX_EXPORT Point DIPToScreenPoint(const Point& dip_point);

GFX_EXPORT Rect ScreenToDIPRect(const Rect& pixel_bounds);

GFX_EXPORT Rect DIPToScreenRect(const Rect& dip_bounds);

GFX_EXPORT Size ScreenToDIPSize(const Size& size_in_pixels);

GFX_EXPORT Size DIPToScreenSize(const Size& dip_size);

GFX_EXPORT int GetSystemMetricsInDIP(int metric);

GFX_EXPORT double GetUndocumentedDPIScale();

GFX_EXPORT double GetUndocumentedDPITouchScale();

}  
}  

#endif  
