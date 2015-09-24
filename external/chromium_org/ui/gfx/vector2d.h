// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_VECTOR2D_H_
#define UI_GFX_VECTOR2D_H_

#include <string>

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/vector2d_f.h"

namespace gfx {

class GFX_EXPORT Vector2d {
 public:
  Vector2d() : x_(0), y_(0) {}
  Vector2d(int x, int y) : x_(x), y_(y) {}

  int x() const { return x_; }
  void set_x(int x) { x_ = x; }

  int y() const { return y_; }
  void set_y(int y) { y_ = y; }

  
  bool IsZero() const;

  
  void Add(const Vector2d& other);
  
  void Subtract(const Vector2d& other);

  void operator+=(const Vector2d& other) { Add(other); }
  void operator-=(const Vector2d& other) { Subtract(other); }

  void SetToMin(const Vector2d& other) {
    x_ = x_ <= other.x_ ? x_ : other.x_;
    y_ = y_ <= other.y_ ? y_ : other.y_;
  }

  void SetToMax(const Vector2d& other) {
    x_ = x_ >= other.x_ ? x_ : other.x_;
    y_ = y_ >= other.y_ ? y_ : other.y_;
  }

  
  
  
  int64 LengthSquared() const;
  
  float Length() const;

  std::string ToString() const;

  operator Vector2dF() const { return Vector2dF(x_, y_); }

 private:
  int x_;
  int y_;
};

inline bool operator==(const Vector2d& lhs, const Vector2d& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

inline Vector2d operator-(const Vector2d& v) {
  return Vector2d(-v.x(), -v.y());
}

inline Vector2d operator+(const Vector2d& lhs, const Vector2d& rhs) {
  Vector2d result = lhs;
  result.Add(rhs);
  return result;
}

inline Vector2d operator-(const Vector2d& lhs, const Vector2d& rhs) {
  Vector2d result = lhs;
  result.Add(-rhs);
  return result;
}

}  

#endif 
