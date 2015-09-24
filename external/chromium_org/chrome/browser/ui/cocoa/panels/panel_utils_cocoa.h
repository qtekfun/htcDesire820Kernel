// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PANELS_PANEL_UTILS_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_PANELS_PANEL_UTILS_COCOA_H_

#import <Cocoa/Cocoa.h>
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

namespace cocoa_utils {

NSRect ConvertRectToCocoaCoordinates(const gfx::Rect& bounds);

gfx::Rect ConvertRectFromCocoaCoordinates(NSRect bounds);

NSPoint ConvertPointToCocoaCoordinates(const gfx::Point& point);

gfx::Point ConvertPointFromCocoaCoordinates(NSPoint point);

}  

#endif  
