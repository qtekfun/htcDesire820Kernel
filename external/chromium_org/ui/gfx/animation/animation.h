// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_ANIMATION_H_
#define UI_GFX_ANIMATION_ANIMATION_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "ui/gfx/animation/animation_container_element.h"

namespace gfx {
class Rect;
}

namespace gfx {

class AnimationContainer;
class AnimationDelegate;

class GFX_EXPORT Animation : public AnimationContainerElement {
 public:
  explicit Animation(base::TimeDelta timer_interval);
  virtual ~Animation();

  
  void Start();

  
  void Stop();

  
  
  virtual double GetCurrentValue() const = 0;

  
  
  double CurrentValueBetween(double start, double target) const;
  int CurrentValueBetween(int start, int target) const;
  gfx::Rect CurrentValueBetween(const gfx::Rect& start_bounds,
                                const gfx::Rect& target_bounds) const;

  
  void set_delegate(AnimationDelegate* delegate) { delegate_ = delegate; }

  
  
  void SetContainer(AnimationContainer* container);

  bool is_animating() const { return is_animating_; }

  base::TimeDelta timer_interval() const { return timer_interval_; }

  
  
  
  static bool ShouldRenderRichAnimation();

 protected:
  
  virtual void AnimationStarted() {}

  
  
  virtual void AnimationStopped() {}

  
  
  
  virtual bool ShouldSendCanceledFromStop();

  AnimationContainer* container() { return container_.get(); }
  base::TimeTicks start_time() const { return start_time_; }
  AnimationDelegate* delegate() { return delegate_; }

  
  virtual void SetStartTime(base::TimeTicks start_time) OVERRIDE;
  virtual void Step(base::TimeTicks time_now) = 0;
  virtual base::TimeDelta GetTimerInterval() const OVERRIDE;

 private:
  
  const base::TimeDelta timer_interval_;

  
  bool is_animating_;

  
  AnimationDelegate* delegate_;

  
  scoped_refptr<AnimationContainer> container_;

  
  base::TimeTicks start_time_;

  DISALLOW_COPY_AND_ASSIGN(Animation);
};

}  

#endif  
