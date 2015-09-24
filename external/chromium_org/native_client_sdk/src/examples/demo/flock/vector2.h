// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <stdlib.h>
#include <cmath>
#include <limits>


class Vector2 {
 public:
  Vector2() : x_(0.0), y_(0.0) {}
  Vector2(double x, double y) : x_(x), y_(y) {}
  ~Vector2() {}

  
  static Vector2 Difference(const Vector2& a, const Vector2& b) {
    Vector2 diff(a.x() - b.x(), a.y() - b.y());
    return diff;
  }

  
  double Magnitude() const {
    return sqrt(x_ * x_ + y_ * y_);
  }

  
  void Add(const Vector2& vec) {
    x_ += vec.x();
    y_ += vec.y();
  }

  
  
  void Normalize() {
    double mag = Magnitude();
    if (fabs(mag) < std::numeric_limits<double>::epsilon())
      return;
    Scale(1.0 / mag);
  }

  
  void Scale(double scale) {
    x_ *= scale;
    y_ *= scale;
  }

  
  
  void Clamp(double max_mag) {
    double mag = Magnitude();
    if (mag > max_mag) {
      Scale(max_mag / mag);  
    }
  }

  
  
  
  double Heading() const {
    double angle = atan2(y_, x_);
    return angle;
  }

  
  double x() const { return x_; }
  void set_x(double x) { x_ = x; }

  double y() const { return y_; }
  void set_y(double y) { y_ = y; }

  double x_;
  double y_;
};

#endif  
