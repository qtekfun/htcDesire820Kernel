// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TRANSFORM_H_
#define UI_GFX_TRANSFORM_H_

#include <string>

#include "base/compiler_specific.h"
#include "third_party/skia/include/utils/SkMatrix44.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/vector2d_f.h"

namespace gfx {

class BoxF;
class RectF;
class Point;
class Point3F;
class Vector3dF;

class GFX_EXPORT Transform {
 public:

  enum SkipInitialization {
    kSkipInitialization
  };

  Transform() : matrix_(SkMatrix44::kIdentity_Constructor) {}

  
  
  Transform(SkipInitialization)
      : matrix_(SkMatrix44::kUninitialized_Constructor) {}
  Transform(const Transform& rhs) : matrix_(rhs.matrix_) {}
  
  Transform(const Transform& lhs, const Transform& rhs)
      : matrix_(lhs.matrix_, rhs.matrix_) {}
  
  
  Transform(SkMScalar col1row1,
            SkMScalar col2row1,
            SkMScalar col3row1,
            SkMScalar col4row1,
            SkMScalar col1row2,
            SkMScalar col2row2,
            SkMScalar col3row2,
            SkMScalar col4row2,
            SkMScalar col1row3,
            SkMScalar col2row3,
            SkMScalar col3row3,
            SkMScalar col4row3,
            SkMScalar col1row4,
            SkMScalar col2row4,
            SkMScalar col3row4,
            SkMScalar col4row4);
  
  
  
  Transform(SkMScalar col1row1,
            SkMScalar col2row1,
            SkMScalar col1row2,
            SkMScalar col2row2,
            SkMScalar x_translation,
            SkMScalar y_translation);
  ~Transform() {}

  bool operator==(const Transform& rhs) const { return matrix_ == rhs.matrix_; }
  bool operator!=(const Transform& rhs) const { return matrix_ != rhs.matrix_; }

  
  void MakeIdentity() { matrix_.setIdentity(); }

  
  
  void Rotate(double degrees) { RotateAboutZAxis(degrees); }

  
  
  void RotateAboutXAxis(double degrees);
  void RotateAboutYAxis(double degrees);
  void RotateAboutZAxis(double degrees);
  void RotateAbout(const Vector3dF& axis, double degrees);

  
  
  void Scale(SkMScalar x, SkMScalar y);
  void Scale3d(SkMScalar x, SkMScalar y, SkMScalar z);

  
  
  void Translate(SkMScalar x, SkMScalar y);
  void Translate3d(SkMScalar x, SkMScalar y, SkMScalar z);

  
  
  void SkewX(double angle_x);
  void SkewY(double angle_y);

  
  
  void ApplyPerspectiveDepth(SkMScalar depth);

  
  
  void PreconcatTransform(const Transform& transform);

  
  
  void ConcatTransform(const Transform& transform);

  
  bool IsIdentity() const { return matrix_.isIdentity(); }

  
  bool IsIdentityOrTranslation() const {
    return !(matrix_.getType() & ~SkMatrix44::kTranslate_Mask);
  }

  
  
  bool IsApproximatelyIdentityOrTranslation(SkMScalar tolerance) const;

  
  bool IsPositiveScaleOrTranslation() const {
    if (!IsScaleOrTranslation())
      return false;
    return matrix_.get(0, 0) > 0.0 && matrix_.get(1, 1) > 0.0 &&
           matrix_.get(2, 2) > 0.0;
  }

  
  
  bool IsIdentityOrIntegerTranslation() const;

  
  bool IsScaleOrTranslation() const {
    int mask = SkMatrix44::kScale_Mask | SkMatrix44::kTranslate_Mask;
    return (matrix_.getType() & ~mask) == 0;
  }

  
  
  bool Preserves2dAxisAlignment() const;

  
  
  bool HasPerspective() const {
    return (matrix_.getType() & SkMatrix44::kPerspective_Mask) != 0;
  }

  
  bool IsInvertible() const { return matrix_.invert(NULL); }

  
  
  bool IsBackFaceVisible() const;

  
  bool GetInverse(Transform* transform) const WARN_UNUSED_RESULT;

  
  void Transpose();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void FlattenTo2d();

  
  Vector2dF To2dTranslation() const;

  
  void TransformPoint(Point3F* point) const;

  
  void TransformPoint(Point* point) const;

  
  
  bool TransformPointReverse(Point3F* point) const;

  
  
  bool TransformPointReverse(Point* point) const;

  
  
  
  void TransformRect(RectF* rect) const;

  
  
  
  
  bool TransformRectReverse(RectF* rect) const;

  
  
  
  void TransformBox(BoxF* box) const;

  
  
  
  
  bool TransformBoxReverse(BoxF* box) const;

  
  
  
  
  
  
  
  
  
  bool Blend(const Transform& from, double progress);

  
  Transform operator*(const Transform& other) const {
    return Transform(*this, other);
  }

  
  Transform& operator*=(const Transform& other) {
    PreconcatTransform(other);
    return *this;
  }

  
  const SkMatrix44& matrix() const { return matrix_; }
  SkMatrix44& matrix() { return matrix_; }

  std::string ToString() const;

 private:
  void TransformPointInternal(const SkMatrix44& xform,
                              Point* point) const;

  void TransformPointInternal(const SkMatrix44& xform,
                              Point3F* point) const;

  SkMatrix44 matrix_;

  
};

}  

#endif  
