// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_GESTURES_GESTURE_UTIL_H_
#define UI_EVENTS_GESTURES_GESTURE_UTIL_H_

#include "ui/events/events_export.h"

namespace gfx {
class Point;
}  

namespace ui {
namespace gestures {

EVENTS_EXPORT bool IsInsideManhattanSquare(const gfx::Point& p1,
                                       const gfx::Point& p2);

}  
}  

#endif  
