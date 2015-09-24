// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_POINT_CONVERSIONS_H_
#define UI_GFX_POINT_CONVERSIONS_H_

#include "ui/gfx/point.h"
#include "ui/gfx/point_f.h"

namespace gfx {

GFX_EXPORT Point ToFlooredPoint(const PointF& point);

GFX_EXPORT Point ToCeiledPoint(const PointF& point);

GFX_EXPORT Point ToRoundedPoint(const PointF& point);

}  

#endif  
