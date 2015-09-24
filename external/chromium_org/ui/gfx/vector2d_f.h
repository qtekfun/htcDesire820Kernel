// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_VECTOR2D_F_H_
#define UI_GFX_VECTOR2D_F_H_

#include <string>

#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT Vector2dF {
 public:
  Vector2dF() : x_(0), y_(0) {}
  Vector2dF(float x, float y) : x_(x), y_(y) {}

  float x() const { return x_; }
  void set_x(float x) { x_ = x; }

  float y() const { return y_; }
  void set_y(float y) { y_ = y; }

  
  bool IsZero() const;

  
  void Add(const Vector2dF& other);
  
  void Subtract(const Vector2dF& other);

  void operator+=(const Vector2dF& other) { Add(other); }
  void operator-=(const Vector2dF& other) { Subtract(other); }

  void SetToMin(const Vector2dF& other) {
    x_ = x_ <= other.x_ ? x_ : other.x_;
    y_ = y_ <= other.y_ ? y_ : other.y_;
  }

  void SetToMax(const Vector2dF& other) {
    x_ = x_ >= other.x_ ? x_ : other.x_;
    y_ = y_ >= other.y_ ? y_ : other.y_;
  }

  
  double LengthSquared() const;
  
  float Length() const;

  
  void Scale(float scale) { Scale(scale, scale); }
  
  
  void Scale(float x_scale, float y_scale);

  std::string ToString() const;

 private:
  float x_;
  float y_;
};

inline bool operator==(const Vector2dF& lhs, const Vector2dF& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

inline bool operator!=(const Vector2dF& lhs, const Vector2dF& rhs) {
  return !(lhs == rhs);
}

inline Vector2dF operator-(const Vector2dF& v) {
  return Vector2dF(-v.x(), -v.y());
}

inline Vector2dF operator+(const Vector2dF& lhs, const Vector2dF& rhs) {
  Vector2dF result = lhs;
  result.Add(rhs);
  return result;
}

inline Vector2dF operator-(const Vector2dF& lhs, const Vector2dF& rhs) {
  Vector2dF result = lhs;
  result.Add(-rhs);
  return result;
}

GFX_EXPORT double CrossProduct(const Vector2dF& lhs, const Vector2dF& rhs);

GFX_EXPORT double DotProduct(const Vector2dF& lhs, const Vector2dF& rhs);

GFX_EXPORT Vector2dF ScaleVector2d(const Vector2dF& v,
                                   float x_scale,
                                   float y_scale);

inline Vector2dF ScaleVector2d(const Vector2dF& v, float scale) {
  return ScaleVector2d(v, scale, scale);
}

}  

#endif 
