// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_ANIMATION_KEYFRAMED_ANIMATION_CURVE_H_
#define CC_ANIMATION_KEYFRAMED_ANIMATION_CURVE_H_

#include "cc/animation/animation_curve.h"
#include "cc/animation/timing_function.h"
#include "cc/animation/transform_operations.h"
#include "cc/base/cc_export.h"
#include "cc/base/scoped_ptr_vector.h"

namespace cc {

class CC_EXPORT Keyframe {
 public:
  double Time() const;
  const TimingFunction* timing_function() const {
    return timing_function_.get();
  }

 protected:
  Keyframe(double time, scoped_ptr<TimingFunction> timing_function);
  virtual ~Keyframe();

 private:
  double time_;
  scoped_ptr<TimingFunction> timing_function_;

  DISALLOW_COPY_AND_ASSIGN(Keyframe);
};

class CC_EXPORT ColorKeyframe : public Keyframe {
 public:
  static scoped_ptr<ColorKeyframe> Create(
      double time,
      SkColor value,
      scoped_ptr<TimingFunction> timing_function);
  virtual ~ColorKeyframe();

  SkColor Value() const;

  scoped_ptr<ColorKeyframe> Clone() const;

 private:
  ColorKeyframe(double time,
                SkColor value,
                scoped_ptr<TimingFunction> timing_function);

  SkColor value_;
};

class CC_EXPORT FloatKeyframe : public Keyframe {
 public:
  static scoped_ptr<FloatKeyframe> Create(
      double time,
      float value,
      scoped_ptr<TimingFunction> timing_function);
  virtual ~FloatKeyframe();

  float Value() const;

  scoped_ptr<FloatKeyframe> Clone() const;

 private:
  FloatKeyframe(double time,
                float value,
                scoped_ptr<TimingFunction> timing_function);

  float value_;
};

class CC_EXPORT TransformKeyframe : public Keyframe {
 public:
  static scoped_ptr<TransformKeyframe> Create(
      double time,
      const TransformOperations& value,
      scoped_ptr<TimingFunction> timing_function);
  virtual ~TransformKeyframe();

  const TransformOperations& Value() const;

  scoped_ptr<TransformKeyframe> Clone() const;

 private:
  TransformKeyframe(
      double time,
      const TransformOperations& value,
      scoped_ptr<TimingFunction> timing_function);

  TransformOperations value_;
};

class CC_EXPORT FilterKeyframe : public Keyframe {
 public:
  static scoped_ptr<FilterKeyframe> Create(
      double time,
      const FilterOperations& value,
      scoped_ptr<TimingFunction> timing_function);
  virtual ~FilterKeyframe();

  const FilterOperations& Value() const;

  scoped_ptr<FilterKeyframe> Clone() const;

 private:
  FilterKeyframe(
      double time,
      const FilterOperations& value,
      scoped_ptr<TimingFunction> timing_function);

  FilterOperations value_;
};

class CC_EXPORT KeyframedColorAnimationCurve : public ColorAnimationCurve {
 public:
  
  static scoped_ptr<KeyframedColorAnimationCurve> Create();

  virtual ~KeyframedColorAnimationCurve();

  void AddKeyframe(scoped_ptr<ColorKeyframe> keyframe);

  
  virtual double Duration() const OVERRIDE;
  virtual scoped_ptr<AnimationCurve> Clone() const OVERRIDE;

  
  virtual SkColor GetValue(double t) const OVERRIDE;

 private:
  KeyframedColorAnimationCurve();

  
  
  ScopedPtrVector<ColorKeyframe> keyframes_;

  DISALLOW_COPY_AND_ASSIGN(KeyframedColorAnimationCurve);
};

class CC_EXPORT KeyframedFloatAnimationCurve : public FloatAnimationCurve {
 public:
  
  static scoped_ptr<KeyframedFloatAnimationCurve> Create();

  virtual ~KeyframedFloatAnimationCurve();

  void AddKeyframe(scoped_ptr<FloatKeyframe> keyframe);

  
  virtual double Duration() const OVERRIDE;
  virtual scoped_ptr<AnimationCurve> Clone() const OVERRIDE;

  
  virtual float GetValue(double t) const OVERRIDE;

 private:
  KeyframedFloatAnimationCurve();

  
  
  ScopedPtrVector<FloatKeyframe> keyframes_;

  DISALLOW_COPY_AND_ASSIGN(KeyframedFloatAnimationCurve);
};

class CC_EXPORT KeyframedTransformAnimationCurve
    : public TransformAnimationCurve {
 public:
  
  static scoped_ptr<KeyframedTransformAnimationCurve> Create();

  virtual ~KeyframedTransformAnimationCurve();

  void AddKeyframe(scoped_ptr<TransformKeyframe> keyframe);

  
  virtual double Duration() const OVERRIDE;
  virtual scoped_ptr<AnimationCurve> Clone() const OVERRIDE;

  
  virtual gfx::Transform GetValue(double t) const OVERRIDE;
  virtual bool AnimatedBoundsForBox(const gfx::BoxF& box,
                                    gfx::BoxF* bounds) const OVERRIDE;

 private:
  KeyframedTransformAnimationCurve();

  
  
  ScopedPtrVector<TransformKeyframe> keyframes_;

  DISALLOW_COPY_AND_ASSIGN(KeyframedTransformAnimationCurve);
};

class CC_EXPORT KeyframedFilterAnimationCurve
    : public FilterAnimationCurve {
 public:
  
  static scoped_ptr<KeyframedFilterAnimationCurve> Create();

  virtual ~KeyframedFilterAnimationCurve();

  void AddKeyframe(scoped_ptr<FilterKeyframe> keyframe);

  
  virtual double Duration() const OVERRIDE;
  virtual scoped_ptr<AnimationCurve> Clone() const OVERRIDE;

  
  virtual FilterOperations GetValue(double t) const OVERRIDE;

 private:
  KeyframedFilterAnimationCurve();

  
  
  ScopedPtrVector<FilterKeyframe> keyframes_;

  DISALLOW_COPY_AND_ASSIGN(KeyframedFilterAnimationCurve);
};

}  

#endif  