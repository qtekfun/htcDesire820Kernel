// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_RECT_H_
#define UI_GFX_RECT_H_

#include <cmath>
#include <string>

#include "ui/gfx/point.h"
#include "ui/gfx/rect_base.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vector2d.h"

#if defined(OS_WIN)
typedef struct tagRECT RECT;
#elif defined(TOOLKIT_GTK)
typedef struct _GdkRectangle GdkRectangle;
#elif defined(OS_IOS)
#include <CoreGraphics/CoreGraphics.h>
#elif defined(OS_MACOSX)
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace gfx {

class Insets;

class GFX_EXPORT Rect
    : public RectBase<Rect, Point, Size, Insets, Vector2d, int> {
 public:
  Rect() : RectBase<Rect, Point, Size, Insets, Vector2d, int>(Point()) {}

  Rect(int width, int height)
      : RectBase<Rect, Point, Size, Insets, Vector2d, int>
            (Size(width, height)) {}

  Rect(int x, int y, int width, int height)
      : RectBase<Rect, Point, Size, Insets, Vector2d, int>
            (Point(x, y), Size(width, height)) {}

#if defined(OS_WIN)
  explicit Rect(const RECT& r);
#elif defined(OS_MACOSX)
  explicit Rect(const CGRect& r);
#elif defined(TOOLKIT_GTK)
  explicit Rect(const GdkRectangle& r);
#endif

  explicit Rect(const gfx::Size& size)
      : RectBase<Rect, Point, Size, Insets, Vector2d, int>(size) {}

  Rect(const gfx::Point& origin, const gfx::Size& size)
      : RectBase<Rect, Point, Size, Insets, Vector2d, int>(origin, size) {}

  ~Rect() {}

#if defined(OS_WIN)
  
  RECT ToRECT() const;
#elif defined(TOOLKIT_GTK)
  GdkRectangle ToGdkRectangle() const;
#elif defined(OS_MACOSX)
  
  CGRect ToCGRect() const;
#endif

  operator RectF() const {
    return RectF(origin().x(), origin().y(), size().width(), size().height());
  }

  std::string ToString() const;
};

inline bool operator==(const Rect& lhs, const Rect& rhs) {
  return lhs.origin() == rhs.origin() && lhs.size() == rhs.size();
}

inline bool operator!=(const Rect& lhs, const Rect& rhs) {
  return !(lhs == rhs);
}

GFX_EXPORT Rect operator+(const Rect& lhs, const Vector2d& rhs);
GFX_EXPORT Rect operator-(const Rect& lhs, const Vector2d& rhs);

inline Rect operator+(const Vector2d& lhs, const Rect& rhs) {
  return rhs + lhs;
}

GFX_EXPORT Rect IntersectRects(const Rect& a, const Rect& b);
GFX_EXPORT Rect UnionRects(const Rect& a, const Rect& b);
GFX_EXPORT Rect SubtractRects(const Rect& a, const Rect& b);

GFX_EXPORT Rect BoundingRect(const Point& p1, const Point& p2);

inline Rect ScaleToEnclosingRect(const Rect& rect,
                                 float x_scale,
                                 float y_scale) {
  int x = std::floor(rect.x() * x_scale);
  int y = std::floor(rect.y() * y_scale);
  int r = rect.width() == 0 ? x : std::ceil(rect.right() * x_scale);
  int b = rect.height() == 0 ? y : std::ceil(rect.bottom() * y_scale);
  return Rect(x, y, r - x, b - y);
}

inline Rect ScaleToEnclosingRect(const Rect& rect, float scale) {
  return ScaleToEnclosingRect(rect, scale, scale);
}

inline Rect ScaleToEnclosedRect(const Rect& rect,
                                float x_scale,
                                float y_scale) {
  int x = std::ceil(rect.x() * x_scale);
  int y = std::ceil(rect.y() * y_scale);
  int r = rect.width() == 0 ? x : std::floor(rect.right() * x_scale);
  int b = rect.height() == 0 ? y : std::floor(rect.bottom() * y_scale);
  return Rect(x, y, r - x, b - y);
}

inline Rect ScaleToEnclosedRect(const Rect& rect, float scale) {
  return ScaleToEnclosedRect(rect, scale, scale);
}

#if !defined(COMPILER_MSVC)
extern template class RectBase<Rect, Point, Size, Insets, Vector2d, int>;
#endif

}  

#endif  
