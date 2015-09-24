// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RECT_CONVERSIONS_H_
#define UI_GFX_RECT_CONVERSIONS_H_

#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"

namespace gfx {

GFX_EXPORT Rect ToEnclosingRect(const RectF& rect);

GFX_EXPORT Rect ToEnclosedRect(const RectF& rect);

GFX_EXPORT Rect ToNearestRect(const RectF& rect);

GFX_EXPORT bool IsNearestRectWithinDistance(
    const gfx::RectF& rect, float distance);

GFX_EXPORT Rect ToFlooredRectDeprecated(const RectF& rect);

}  

#endif  
