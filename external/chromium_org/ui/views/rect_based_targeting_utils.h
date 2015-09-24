// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_RECT_BASED_TARGETING_UTILS_H_
#define UI_RECT_BASED_TARGETING_UTILS_H_

#include "ui/views/views_export.h"

namespace gfx {
class Point;
class Rect;
}

namespace views {

VIEWS_EXPORT bool UsePointBasedTargeting(const gfx::Rect& rect);

VIEWS_EXPORT float PercentCoveredBy(const gfx::Rect& rect_1,
                                    const gfx::Rect& rect_2);

VIEWS_EXPORT int DistanceSquaredFromCenterToPoint(const gfx::Point& point,
                                                  const gfx::Rect& rect);

}  

#endif  
