// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_VECTOR2D_CONVERSIONS_H_
#define UI_GFX_VECTOR2D_CONVERSIONS_H_

#include "ui/gfx/vector2d.h"
#include "ui/gfx/vector2d_f.h"

namespace gfx {

GFX_EXPORT Vector2d ToFlooredVector2d(const Vector2dF& vector2d);

GFX_EXPORT Vector2d ToCeiledVector2d(const Vector2dF& vector2d);

GFX_EXPORT Vector2d ToRoundedVector2d(const Vector2dF& vector2d);

}  

#endif  
