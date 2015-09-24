// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_ANIMATION_ELEMENT_H_
#define UI_COMPOSITOR_LAYER_ANIMATION_ELEMENT_H_

#include <set>

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "cc/animation/animation.h"
#include "cc/animation/animation_events.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/compositor/compositor_export.h"
#include "ui/gfx/animation/tween.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

namespace ui {

class InterpolatedTransform;
class LayerAnimationDelegate;

class COMPOSITOR_EXPORT LayerAnimationElement {
 public:
  enum AnimatableProperty {
    TRANSFORM = 0,
    BOUNDS,
    OPACITY,
    VISIBILITY,
    BRIGHTNESS,
    GRAYSCALE,
    COLOR,
  };

  static AnimatableProperty ToAnimatableProperty(
      cc::Animation::TargetProperty property);

  struct COMPOSITOR_EXPORT TargetValue {
    TargetValue();
    
    explicit TargetValue(const LayerAnimationDelegate* delegate);

    gfx::Rect bounds;
    gfx::Transform transform;
    float opacity;
    bool visibility;
    float brightness;
    float grayscale;
    SkColor color;
  };

  typedef std::set<AnimatableProperty> AnimatableProperties;

  LayerAnimationElement(const AnimatableProperties& properties,
                        base::TimeDelta duration);

  virtual ~LayerAnimationElement();

  
  
  static LayerAnimationElement* CreateTransformElement(
      const gfx::Transform& transform,
      base::TimeDelta duration);

  
  
  
  
  static LayerAnimationElement* CreateInverseTransformElement(
      const gfx::Transform& base_transform,
      const LayerAnimationElement* uninverted_transition);


  
  static LayerAnimationElement* CloneInverseTransformElement(
      const LayerAnimationElement* other);

  
  
  
  
  
  
  
  static LayerAnimationElement* CreateInterpolatedTransformElement(
      InterpolatedTransform* interpolated_transform,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreateBoundsElement(
      const gfx::Rect& bounds,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreateOpacityElement(
      float opacity,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreateVisibilityElement(
      bool visibility,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreateBrightnessElement(
      float brightness,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreateGrayscaleElement(
      float grayscale,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreatePauseElement(
      const AnimatableProperties& properties,
      base::TimeDelta duration);

  
  
  static LayerAnimationElement* CreateColorElement(
      SkColor color,
      base::TimeDelta duration);

  
  
  
  void set_requested_start_time(base::TimeTicks start_time) {
    requested_start_time_ = start_time;
  }
  base::TimeTicks requested_start_time() const { return requested_start_time_; }

  
  
  void set_effective_start_time(base::TimeTicks start_time) {
    effective_start_time_ = start_time;
  }
  base::TimeTicks effective_start_time() const { return effective_start_time_; }

  
  
  
  
  void Start(LayerAnimationDelegate* delegate, int animation_group_id);

  
  bool Started() { return !first_frame_; }

  
  
  bool Progress(base::TimeTicks now, LayerAnimationDelegate* delegate);

  
  
  
  bool IsFinished(base::TimeTicks time, base::TimeDelta* total_duration);

  
  
  bool ProgressToEnd(LayerAnimationDelegate* delegate);

  
  
  void Abort(LayerAnimationDelegate* delegate);

  
  void GetTargetValue(TargetValue* target) const;

  
  const AnimatableProperties& properties() const { return properties_; }

  
  virtual bool IsThreaded() const;

  gfx::Tween::Type tween_type() const { return tween_type_; }
  void set_tween_type(gfx::Tween::Type tween_type) { tween_type_ = tween_type; }

  
  
  
  int animation_id() const { return animation_id_; }
  int animation_group_id() const { return animation_group_id_; }
  void set_animation_group_id(int id) { animation_group_id_ = id; }

  base::TimeDelta duration() const { return duration_; }

  
  
  double last_progressed_fraction() const { return last_progressed_fraction_; }

 protected:
  
  
  virtual void OnStart(LayerAnimationDelegate* delegate) = 0;
  virtual bool OnProgress(double t, LayerAnimationDelegate* delegate) = 0;
  virtual void OnGetTarget(TargetValue* target) const = 0;
  virtual void OnAbort(LayerAnimationDelegate* delegate) = 0;

  
  virtual void RequestEffectiveStart(LayerAnimationDelegate* delegate);

  LayerAnimationElement(const LayerAnimationElement& element);

 private:
  
  
  
  base::TimeDelta GetEffectiveDuration(const base::TimeDelta& delta);

  bool first_frame_;
  const AnimatableProperties properties_;
  base::TimeTicks requested_start_time_;

  
  base::TimeTicks effective_start_time_;
  const base::TimeDelta duration_;
  gfx::Tween::Type tween_type_;

  const int animation_id_;
  int animation_group_id_;

  double last_progressed_fraction_;

  base::WeakPtrFactory<LayerAnimationElement> weak_ptr_factory_;

  DISALLOW_ASSIGN(LayerAnimationElement);
};

}  

#endif  
