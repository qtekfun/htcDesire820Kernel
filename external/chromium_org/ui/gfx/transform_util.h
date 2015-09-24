// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TRANSFORM_UTIL_H_
#define UI_GFX_TRANSFORM_UTIL_H_

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/transform.h"

namespace gfx {

class Point;
class Rect;

GFX_EXPORT Transform GetScaleTransform(const Point& anchor, float scale);

struct GFX_EXPORT DecomposedTransform {
  
  
  DecomposedTransform();

  SkMScalar translate[3];
  SkMScalar scale[3];
  SkMScalar skew[3];
  SkMScalar perspective[4];
  SkMScalar quaternion[4];

  std::string ToString() const;

  
};

GFX_EXPORT bool BlendDecomposedTransforms(DecomposedTransform* out,
                                          const DecomposedTransform& to,
                                          const DecomposedTransform& from,
                                          double progress);

GFX_EXPORT bool DecomposeTransform(DecomposedTransform* out,
                                   const Transform& transform);

GFX_EXPORT Transform ComposeTransform(const DecomposedTransform& decomp);

GFX_EXPORT bool SnapTransform(Transform* out,
                              const Transform& transform,
                              const Rect& viewport);

}  

#endif  
