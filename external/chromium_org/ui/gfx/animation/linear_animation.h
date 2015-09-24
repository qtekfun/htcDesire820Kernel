// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_LINEAR_ANIMATION_H_
#define UI_GFX_ANIMATION_LINEAR_ANIMATION_H_

#include "base/time/time.h"
#include "ui/gfx/animation/animation.h"

namespace gfx {

class AnimationDelegate;

class GFX_EXPORT LinearAnimation : public Animation {
 public:
  
  
  
  
  
  
  LinearAnimation(int frame_rate, AnimationDelegate* delegate);

  
  LinearAnimation(int duration, int frame_rate, AnimationDelegate* delegate);

  
  
  
  virtual double GetCurrentValue() const OVERRIDE;

  
  void SetCurrentValue(double new_value);

  
  void End();

  
  
  void SetDuration(int duration);

 protected:
  
  
  virtual void AnimateToState(double state) {}

  
  
  
  virtual void Step(base::TimeTicks time_now) OVERRIDE;

  
  virtual void AnimationStarted() OVERRIDE;

  
  virtual void AnimationStopped() OVERRIDE;

  
  virtual bool ShouldSendCanceledFromStop() OVERRIDE;

 private:
  base::TimeDelta duration_;

  
  double state_;

  
  
  bool in_end_;

  DISALLOW_COPY_AND_ASSIGN(LinearAnimation);
};

}  

#endif  
