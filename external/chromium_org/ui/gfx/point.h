// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_POINT_H_
#define UI_GFX_POINT_H_

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/point_base.h"
#include "ui/gfx/point_f.h"
#include "ui/gfx/vector2d.h"

#if defined(OS_WIN)
typedef unsigned long DWORD;
typedef struct tagPOINT POINT;
#elif defined(OS_IOS)
#include <CoreGraphics/CoreGraphics.h>
#elif defined(OS_MACOSX)
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace gfx {

class GFX_EXPORT Point : public PointBase<Point, int, Vector2d> {
 public:
  Point() : PointBase<Point, int, Vector2d>(0, 0) {}
  Point(int x, int y) : PointBase<Point, int, Vector2d>(x, y) {}
#if defined(OS_WIN)
  
  
  
  explicit Point(DWORD point);
  explicit Point(const POINT& point);
  Point& operator=(const POINT& point);
#elif defined(OS_MACOSX)
  explicit Point(const CGPoint& point);
#endif

  ~Point() {}

#if defined(OS_WIN)
  POINT ToPOINT() const;
#elif defined(OS_MACOSX)
  CGPoint ToCGPoint() const;
#endif

  operator PointF() const {
    return PointF(x(), y());
  }

  
  std::string ToString() const;
};

inline bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

inline bool operator!=(const Point& lhs, const Point& rhs) {
  return !(lhs == rhs);
}

inline Point operator+(const Point& lhs, const Vector2d& rhs) {
  Point result(lhs);
  result += rhs;
  return result;
}

inline Point operator-(const Point& lhs, const Vector2d& rhs) {
  Point result(lhs);
  result -= rhs;
  return result;
}

inline Vector2d operator-(const Point& lhs, const Point& rhs) {
  return Vector2d(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

inline Point PointAtOffsetFromOrigin(const Vector2d& offset_from_origin) {
  return Point(offset_from_origin.x(), offset_from_origin.y());
}

#if !defined(COMPILER_MSVC)
extern template class PointBase<Point, int, Vector2d>;
#endif

}  

#endif  
