// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_COLOR_UTILS_H_
#define UI_GFX_COLOR_UTILS_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/gfx_export.h"

class SkBitmap;

namespace color_utils {

struct HSL {
  double h;
  double s;
  double l;
};

GFX_EXPORT unsigned char GetLuminanceForColor(SkColor color);

GFX_EXPORT double RelativeLuminance(SkColor color);

GFX_EXPORT void SkColorToHSL(SkColor c, HSL* hsl);
GFX_EXPORT SkColor HSLToSkColor(const HSL& hsl, SkAlpha alpha);

GFX_EXPORT SkColor HSLShift(SkColor color, const HSL& shift);

GFX_EXPORT void BuildLumaHistogram(const SkBitmap& bitmap, int histogram[256]);

GFX_EXPORT SkColor AlphaBlend(SkColor foreground, SkColor background,
                              SkAlpha alpha);

GFX_EXPORT SkColor BlendTowardOppositeLuminance(SkColor color, SkAlpha alpha);

GFX_EXPORT SkColor GetReadableColor(SkColor foreground, SkColor background);

GFX_EXPORT SkColor InvertColor(SkColor color);

GFX_EXPORT SkColor GetSysSkColor(int which);

}  

#endif  
