// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_ANIMATOR_H_
#define UI_COMPOSITOR_LAYER_ANIMATOR_H_

#include <deque>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "ui/compositor/compositor_export.h"
#include "ui/compositor/layer_animation_element.h"
#include "ui/gfx/animation/animation_container_element.h"
#include "ui/gfx/animation/tween.h"

namespace gfx {
class Animation;
class Rect;
class Transform;
}

namespace ui {
class Layer;
class LayerAnimationSequence;
class LayerAnimationDelegate;
class LayerAnimationObserver;
class ScopedLayerAnimationSettings;

class COMPOSITOR_EXPORT LayerAnimator
    : public gfx::AnimationContainerElement,
      public base::RefCounted<LayerAnimator> {
 public:
  enum PreemptionStrategy {
    IMMEDIATELY_SET_NEW_TARGET,
    IMMEDIATELY_ANIMATE_TO_NEW_TARGET,
    ENQUEUE_NEW_ANIMATION,
    REPLACE_QUEUED_ANIMATIONS,
    BLEND_WITH_CURRENT_ANIMATION
  };

  explicit LayerAnimator(base::TimeDelta transition_duration);

  
  static LayerAnimator* CreateDefaultAnimator();

  
  static LayerAnimator* CreateImplicitAnimator();

  
  virtual void SetTransform(const gfx::Transform& transform);
  gfx::Transform GetTargetTransform() const;

  
  virtual void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetTargetBounds() const;

  
  virtual void SetOpacity(float opacity);
  float GetTargetOpacity() const;

  
  virtual void SetVisibility(bool visibility);
  bool GetTargetVisibility() const;

  
  virtual void SetBrightness(float brightness);
  float GetTargetBrightness() const;

  
  virtual void SetGrayscale(float grayscale);
  float GetTargetGrayscale() const;

  
  virtual void SetColor(SkColor color);
  SkColor GetTargetColor() const;

  
  
  base::TimeDelta GetTransitionDuration() const;

  
  
  
  
  void SetDelegate(LayerAnimationDelegate* delegate);

  
  
  
  void set_preemption_strategy(PreemptionStrategy strategy) {
    preemption_strategy_ = strategy;
  }

  PreemptionStrategy preemption_strategy() const {
    return preemption_strategy_;
  }

  
  
  
  void StartAnimation(LayerAnimationSequence* animation);

  
  
  void ScheduleAnimation(LayerAnimationSequence* animation);

  
  
  
  
  
  
  void StartTogether(const std::vector<LayerAnimationSequence*>& animations);

  
  
  
  
  
  
  void ScheduleTogether(const std::vector<LayerAnimationSequence*>& animations);

  
  
  void SchedulePauseForProperties(
      base::TimeDelta duration,
      LayerAnimationElement::AnimatableProperty property,
      ...);

  
  
  bool is_animating() const { return !animation_queue_.empty(); }

  
  
  
  bool IsAnimatingProperty(
      LayerAnimationElement::AnimatableProperty property) const;

  
  
  
  void StopAnimatingProperty(
      LayerAnimationElement::AnimatableProperty property);

  
  
  void StopAnimating() { StopAnimatingInternal(false); }

  
  
  void AbortAllAnimations() { StopAnimatingInternal(true); }

  
  
  void AddObserver(LayerAnimationObserver* observer);
  void RemoveObserver(LayerAnimationObserver* observer);

  
  void OnThreadedAnimationStarted(const cc::AnimationEvent& event);

  
  
  
  void set_tween_type(gfx::Tween::Type tween_type) { tween_type_ = tween_type; }
  gfx::Tween::Type tween_type() const { return tween_type_; }

  
  void set_disable_timer_for_test(bool disable_timer) {
    disable_timer_for_test_ = disable_timer;
  }

  void set_last_step_time(base::TimeTicks time) {
    last_step_time_ = time;
  }
  base::TimeTicks last_step_time() const { return last_step_time_; }

 protected:
  virtual ~LayerAnimator();

  LayerAnimationDelegate* delegate() { return delegate_; }
  const LayerAnimationDelegate* delegate() const { return delegate_; }

  
  virtual void ProgressAnimation(LayerAnimationSequence* sequence,
                                 base::TimeTicks now);

  void ProgressAnimationToEnd(LayerAnimationSequence* sequence);

  
  bool HasAnimation(LayerAnimationSequence* sequence) const;

 private:
  friend class base::RefCounted<LayerAnimator>;
  friend class ScopedLayerAnimationSettings;
  friend class LayerAnimatorTestController;

  class RunningAnimation {
   public:
    RunningAnimation(const base::WeakPtr<LayerAnimationSequence>& sequence);
    ~RunningAnimation();

    bool is_sequence_alive() const { return !!sequence_.get(); }
    LayerAnimationSequence* sequence() const { return sequence_.get(); }

   private:
    base::WeakPtr<LayerAnimationSequence> sequence_;

    
  };

  typedef std::vector<RunningAnimation> RunningAnimations;
  typedef std::deque<linked_ptr<LayerAnimationSequence> > AnimationQueue;

  
  virtual void SetStartTime(base::TimeTicks start_time) OVERRIDE;
  virtual void Step(base::TimeTicks time_now) OVERRIDE;
  virtual base::TimeDelta GetTimerInterval() const OVERRIDE;

  
  
  void StopAnimatingInternal(bool abort);

  
  void UpdateAnimationState();

  
  
  
  LayerAnimationSequence* RemoveAnimation(
      LayerAnimationSequence* sequence) WARN_UNUSED_RESULT;

  
  void FinishAnimation(LayerAnimationSequence* sequence, bool abort);

  
  void FinishAnyAnimationWithZeroDuration();

  
  void ClearAnimations();

  
  RunningAnimation* GetRunningAnimation(
      LayerAnimationElement::AnimatableProperty property);

  
  
  void AddToQueueIfNotPresent(LayerAnimationSequence* sequence);

  
  
  void RemoveAllAnimationsWithACommonProperty(LayerAnimationSequence* sequence,
                                              bool abort);

  
  
  void ImmediatelySetNewTarget(LayerAnimationSequence* sequence);

  
  void ImmediatelyAnimateToNewTarget(LayerAnimationSequence* sequence);

  
  void EnqueueNewAnimation(LayerAnimationSequence* sequence);

  
  
  void ReplaceQueuedAnimations(LayerAnimationSequence* sequence);

  
  
  
  void ProcessQueue();

  
  
  
  bool StartSequenceImmediately(LayerAnimationSequence* sequence);

  
  
  void GetTargetValue(LayerAnimationElement::TargetValue* target) const;

  
  
  void OnScheduled(LayerAnimationSequence* sequence);

  
  void SetTransitionDuration(base::TimeDelta duration);

  
  
  void ClearAnimationsInternal();

  
  void PurgeDeletedAnimations();

  
  AnimationQueue animation_queue_;

  
  LayerAnimationDelegate* delegate_;

  
  RunningAnimations running_animations_;

  
  PreemptionStrategy preemption_strategy_;

  
  
  bool is_transition_duration_locked_;

  
  base::TimeDelta transition_duration_;

  
  gfx::Tween::Type tween_type_;

  
  base::TimeTicks last_step_time_;

  
  bool is_started_;

  
  
  bool disable_timer_for_test_;

  
  
  bool adding_animations_;

  
  
  ObserverList<LayerAnimationObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(LayerAnimator);
};

}  

#endif  
