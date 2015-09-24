// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_RECT_BASE_H_
#define UI_GFX_RECT_BASE_H_

#include <string>

#include "base/compiler_specific.h"

namespace gfx {

template<typename Class,
         typename PointClass,
         typename SizeClass,
         typename InsetsClass,
         typename VectorClass,
         typename Type>
class GFX_EXPORT RectBase {
 public:
  Type x() const { return origin_.x(); }
  void set_x(Type x) { origin_.set_x(x); }

  Type y() const { return origin_.y(); }
  void set_y(Type y) { origin_.set_y(y); }

  Type width() const { return size_.width(); }
  void set_width(Type width) { size_.set_width(width); }

  Type height() const { return size_.height(); }
  void set_height(Type height) { size_.set_height(height); }

  const PointClass& origin() const { return origin_; }
  void set_origin(const PointClass& origin) { origin_ = origin; }

  const SizeClass& size() const { return size_; }
  void set_size(const SizeClass& size) { size_ = size; }

  Type right() const { return x() + width(); }
  Type bottom() const { return y() + height(); }

  PointClass top_right() const { return PointClass(right(), y()); }
  PointClass bottom_left() const { return PointClass(x(), bottom()); }
  PointClass bottom_right() const { return PointClass(right(), bottom()); }

  VectorClass OffsetFromOrigin() const {
    return VectorClass(x(), y());
  }

  void SetRect(Type x, Type y, Type width, Type height);

  
  void Inset(Type horizontal, Type vertical) {
    Inset(horizontal, vertical, horizontal, vertical);
  }

  
  void Inset(const InsetsClass& insets);

  
  void Inset(Type left, Type top, Type right, Type bottom);

  
  void Offset(Type horizontal, Type vertical);
  void Offset(const VectorClass& distance) {
    Offset(distance.x(), distance.y());
  }
  void operator+=(const VectorClass& offset);
  void operator-=(const VectorClass& offset);

  InsetsClass InsetsFrom(const Class& inner) const {
    return InsetsClass(inner.y() - y(),
                       inner.x() - x(),
                       bottom() - inner.bottom(),
                       right() - inner.right());
  }

  
  bool IsEmpty() const { return size_.IsEmpty(); }

  
  
  
  
  
  
  bool operator<(const Class& other) const;

  
  
  
  bool Contains(Type point_x, Type point_y) const;

  
  bool Contains(const PointClass& point) const {
    return Contains(point.x(), point.y());
  }

  
  bool Contains(const Class& rect) const;

  
  
  bool Intersects(const Class& rect) const;

  
  void Intersect(const Class& rect);

  
  
  void Union(const Class& rect);

  
  
  void Subtract(const Class& rect);

  
  
  
  
  
  void AdjustToFit(const Class& rect);

  
  PointClass CenterPoint() const;

  
  
  void ClampToCenteredSize(const SizeClass& size);

  
  void SplitVertically(Class* left_half, Class* right_half) const;

  
  
  bool SharesEdgeWith(const Class& rect) const;

  
  
  Type ManhattanDistanceToPoint(const PointClass& point) const;

  
  
  
  
  Type ManhattanInternalDistance(const Class& rect) const;

 protected:
  RectBase(const PointClass& origin, const SizeClass& size)
      : origin_(origin), size_(size) {}
  explicit RectBase(const SizeClass& size)
      : size_(size) {}
  explicit RectBase(const PointClass& origin)
      : origin_(origin) {}
  
  
  ~RectBase() {}

 private:
  PointClass origin_;
  SizeClass size_;
};

}  

#endif  
