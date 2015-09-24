// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SKIA_UTIL_H_
#define UI_GFX_SKIA_UTIL_H_

#include <string>
#include <vector>

#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkRect.h"
#include "third_party/skia/include/core/SkShader.h"
#include "ui/gfx/gfx_export.h"

class SkBitmap;
class SkDrawLooper;

namespace gfx {

class ImageSkiaRep;
class Rect;
class RectF;
class ShadowValue;
class Transform;

GFX_EXPORT SkRect RectToSkRect(const Rect& rect);
GFX_EXPORT SkIRect RectToSkIRect(const Rect& rect);
GFX_EXPORT Rect SkIRectToRect(const SkIRect& rect);
GFX_EXPORT SkRect RectFToSkRect(const RectF& rect);
GFX_EXPORT RectF SkRectToRectF(const SkRect& rect);

GFX_EXPORT void TransformToFlattenedSkMatrix(const gfx::Transform& transform,
                                             SkMatrix* flattened);

GFX_EXPORT skia::RefPtr<SkShader> CreateImageRepShader(
    const gfx::ImageSkiaRep& image_rep,
    SkShader::TileMode tile_mode,
    const SkMatrix& local_matrix);

GFX_EXPORT skia::RefPtr<SkShader> CreateGradientShader(int start_point,
                                                       int end_point,
                                                       SkColor start_color,
                                                       SkColor end_color);

GFX_EXPORT skia::RefPtr<SkDrawLooper> CreateShadowDrawLooper(
    const std::vector<ShadowValue>& shadows);

GFX_EXPORT bool BitmapsAreEqual(const SkBitmap& bitmap1,
                                const SkBitmap& bitmap2);

GFX_EXPORT void ConvertSkiaToRGBA(const unsigned char* skia,
                                  int pixel_width,
                                  unsigned char* rgba);

}  

#endif  
