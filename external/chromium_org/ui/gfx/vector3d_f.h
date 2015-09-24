// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_VECTOR3D_F_H_
#define UI_GFX_VECTOR3D_F_H_

#include <string>

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/vector2d_f.h"

namespace gfx {

class GFX_EXPORT Vector3dF {
 public:
  Vector3dF();
  Vector3dF(float x, float y, float z);

  explicit Vector3dF(const Vector2dF& other);

  float x() const { return x_; }
  void set_x(float x) { x_ = x; }

  float y() const { return y_; }
  void set_y(float y) { y_ = y; }

  float z() const { return z_; }
  void set_z(float z) { z_ = z; }

  
  bool IsZero() const;

  
  void Add(const Vector3dF& other);
  
  void Subtract(const Vector3dF& other);

  void operator+=(const Vector3dF& other) { Add(other); }
  void operator-=(const Vector3dF& other) { Subtract(other); }

  void SetToMin(const Vector3dF& other) {
    x_ = x_ <= other.x_ ? x_ : other.x_;
    y_ = y_ <= other.y_ ? y_ : other.y_;
    z_ = z_ <= other.z_ ? z_ : other.z_;
  }

  void SetToMax(const Vector3dF& other) {
    x_ = x_ >= other.x_ ? x_ : other.x_;
    y_ = y_ >= other.y_ ? y_ : other.y_;
    z_ = z_ >= other.z_ ? z_ : other.z_;
  }

  
  double LengthSquared() const;
  
  float Length() const;

  
  void Scale(float scale) { Scale(scale, scale, scale); }
  
  void Scale(float x_scale, float y_scale, float z_scale);

  
  void Cross(const Vector3dF& other);

  std::string ToString() const;

 private:
  float x_;
  float y_;
  float z_;
};

inline bool operator==(const Vector3dF& lhs, const Vector3dF& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

inline Vector3dF operator-(const Vector3dF& v) {
  return Vector3dF(-v.x(), -v.y(), -v.z());
}

inline Vector3dF operator+(const Vector3dF& lhs, const Vector3dF& rhs) {
  Vector3dF result = lhs;
  result.Add(rhs);
  return result;
}

inline Vector3dF operator-(const Vector3dF& lhs, const Vector3dF& rhs) {
  Vector3dF result = lhs;
  result.Add(-rhs);
  return result;
}

inline Vector3dF CrossProduct(const Vector3dF& lhs, const Vector3dF& rhs) {
  Vector3dF result = lhs;
  result.Cross(rhs);
  return result;
}

GFX_EXPORT float DotProduct(const Vector3dF& lhs, const Vector3dF& rhs);

GFX_EXPORT Vector3dF ScaleVector3d(const Vector3dF& v,
                                   float x_scale,
                                   float y_scale,
                                   float z_scale);

inline Vector3dF ScaleVector3d(const Vector3dF& v, float scale) {
  return ScaleVector3d(v, scale, scale, scale);
}

}  

#endif 
