// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RECT_F_H_
#define UI_GFX_RECT_F_H_

#include <string>

#include "ui/gfx/point_f.h"
#include "ui/gfx/rect_base.h"
#include "ui/gfx/size_f.h"
#include "ui/gfx/vector2d_f.h"

namespace gfx {

class InsetsF;

class GFX_EXPORT RectF
    : public RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float> {
 public:
  RectF()
      : RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float>
            (SizeF()) {}

  RectF(float width, float height)
      : RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float>
            (SizeF(width, height)) {}

  RectF(float x, float y, float width, float height)
      : RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float>
            (PointF(x, y), SizeF(width, height)) {}

  explicit RectF(const SizeF& size)
      : RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float>
            (size) {}

  RectF(const PointF& origin, const SizeF& size)
      : RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float>
            (origin, size) {}

  ~RectF() {}

  
  void Scale(float scale) {
    Scale(scale, scale);
  }

  void Scale(float x_scale, float y_scale) {
    set_origin(ScalePoint(origin(), x_scale, y_scale));
    set_size(ScaleSize(size(), x_scale, y_scale));
  }

  
  
  
  
  bool IsExpressibleAsRect() const;

  std::string ToString() const;
};

inline bool operator==(const RectF& lhs, const RectF& rhs) {
  return lhs.origin() == rhs.origin() && lhs.size() == rhs.size();
}

inline bool operator!=(const RectF& lhs, const RectF& rhs) {
  return !(lhs == rhs);
}

inline RectF operator+(const RectF& lhs, const Vector2dF& rhs) {
  return RectF(lhs.x() + rhs.x(), lhs.y() + rhs.y(),
      lhs.width(), lhs.height());
}

inline RectF operator-(const RectF& lhs, const Vector2dF& rhs) {
  return RectF(lhs.x() - rhs.x(), lhs.y() - rhs.y(),
      lhs.width(), lhs.height());
}

inline RectF operator+(const Vector2dF& lhs, const RectF& rhs) {
  return rhs + lhs;
}

GFX_EXPORT RectF IntersectRects(const RectF& a, const RectF& b);
GFX_EXPORT RectF UnionRects(const RectF& a, const RectF& b);
GFX_EXPORT RectF SubtractRects(const RectF& a, const RectF& b);

inline RectF ScaleRect(const RectF& r, float x_scale, float y_scale) {
  return RectF(r.x() * x_scale, r.y() * y_scale,
       r.width() * x_scale, r.height() * y_scale);
}

inline RectF ScaleRect(const RectF& r, float scale) {
  return ScaleRect(r, scale, scale);
}

GFX_EXPORT RectF BoundingRect(const PointF& p1, const PointF& p2);

#if !defined(COMPILER_MSVC)
extern template class RectBase<RectF, PointF, SizeF, InsetsF, Vector2dF, float>;
#endif

}  

#endif  
