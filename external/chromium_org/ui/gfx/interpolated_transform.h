// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_INTERPOLATED_TRANSFORM_H_
#define UI_GFX_INTERPOLATED_TRANSFORM_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/point.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/transform.h"
#include "ui/gfx/transform_util.h"
#include "ui/gfx/vector3d_f.h"

namespace ui {

class GFX_EXPORT InterpolatedTransform {
 public:
  InterpolatedTransform();
  
  
  
  InterpolatedTransform(float start_time, float end_time);
  virtual ~InterpolatedTransform();

  
  gfx::Transform Interpolate(float t) const;

  
  
  
  
  void SetChild(InterpolatedTransform* child);

  
  
  void SetReversed(bool reversed) { reversed_ = reversed; }
  bool Reversed() const { return reversed_; }

 protected:
  
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const = 0;

  
  
  
  
  
  
  float ValueBetween(float time, float start_value, float end_value) const;

  float start_time() const { return start_time_; }
  float end_time() const { return end_time_; }

 private:
  const float start_time_;
  const float end_time_;

  
  
  
  
  scoped_ptr<InterpolatedTransform> child_;

  bool reversed_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedTransform);
};

class GFX_EXPORT InterpolatedRotation : public InterpolatedTransform {
 public:
  InterpolatedRotation(float start_degrees, float end_degrees);
  InterpolatedRotation(float start_degrees,
                       float end_degrees,
                       float start_time,
                       float end_time);
  virtual ~InterpolatedRotation();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  const float start_degrees_;
  const float end_degrees_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedRotation);
};

class GFX_EXPORT InterpolatedAxisAngleRotation : public InterpolatedTransform {
 public:
  InterpolatedAxisAngleRotation(const gfx::Vector3dF& axis,
                                float start_degrees,
                                float end_degrees);
  InterpolatedAxisAngleRotation(const gfx::Vector3dF& axis,
                                float start_degrees,
                                float end_degrees,
                                float start_time,
                                float end_time);
  virtual ~InterpolatedAxisAngleRotation();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  gfx::Vector3dF axis_;
  const float start_degrees_;
  const float end_degrees_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedAxisAngleRotation);
};

class GFX_EXPORT InterpolatedScale : public InterpolatedTransform {
 public:
  InterpolatedScale(float start_scale, float end_scale);
  InterpolatedScale(float start_scale, float end_scale,
                    float start_time, float end_time);
  InterpolatedScale(const gfx::Point3F& start_scale,
                    const gfx::Point3F& end_scale);
  InterpolatedScale(const gfx::Point3F& start_scale,
                    const gfx::Point3F& end_scale,
                    float start_time,
                    float end_time);
  virtual ~InterpolatedScale();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  const gfx::Point3F start_scale_;
  const gfx::Point3F end_scale_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedScale);
};

class GFX_EXPORT InterpolatedTranslation : public InterpolatedTransform {
 public:
  InterpolatedTranslation(const gfx::Point& start_pos,
                          const gfx::Point& end_pos);
  InterpolatedTranslation(const gfx::Point& start_pos,
                          const gfx::Point& end_pos,
                          float start_time,
                          float end_time);
  virtual ~InterpolatedTranslation();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  const gfx::Point start_pos_;
  const gfx::Point end_pos_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedTranslation);
};

class GFX_EXPORT InterpolatedConstantTransform : public InterpolatedTransform {
 public:
  explicit InterpolatedConstantTransform(const gfx::Transform& transform);
  virtual ~InterpolatedConstantTransform();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  const gfx::Transform transform_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedConstantTransform);
};

class GFX_EXPORT InterpolatedTransformAboutPivot
    : public InterpolatedTransform {
 public:
  
  InterpolatedTransformAboutPivot(const gfx::Point& pivot,
                                  InterpolatedTransform* transform);

  
  InterpolatedTransformAboutPivot(const gfx::Point& pivot,
                                  InterpolatedTransform* transform,
                                  float start_time,
                                  float end_time);
  virtual ~InterpolatedTransformAboutPivot();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  void Init(const gfx::Point& pivot, InterpolatedTransform* transform);

  scoped_ptr<InterpolatedTransform> transform_;

  DISALLOW_COPY_AND_ASSIGN(InterpolatedTransformAboutPivot);
};

class GFX_EXPORT InterpolatedMatrixTransform : public InterpolatedTransform {
 public:
  InterpolatedMatrixTransform(const gfx::Transform& start_transform,
                              const gfx::Transform& end_transform);

  InterpolatedMatrixTransform(const gfx::Transform& start_transform,
                              const gfx::Transform& end_transform,
                              float start_time,
                              float end_time);

  virtual ~InterpolatedMatrixTransform();

 protected:
  virtual gfx::Transform InterpolateButDoNotCompose(float t) const OVERRIDE;

 private:
  void Init(const gfx::Transform& start_transform,
            const gfx::Transform& end_transform);

  gfx::DecomposedTransform start_decomp_;
  gfx::DecomposedTransform end_decomp_;
};

} 

#endif  
