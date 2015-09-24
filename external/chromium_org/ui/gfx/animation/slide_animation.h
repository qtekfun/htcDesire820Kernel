// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_SLIDE_ANIMATION_H_
#define UI_GFX_ANIMATION_SLIDE_ANIMATION_H_

#include "ui/gfx/animation/linear_animation.h"
#include "ui/gfx/animation/tween.h"

namespace gfx {

class GFX_EXPORT SlideAnimation : public LinearAnimation {
 public:
  explicit SlideAnimation(AnimationDelegate* target);
  virtual ~SlideAnimation();

  
  virtual void Reset();
  virtual void Reset(double value);

  
  virtual void Show();

  
  virtual void Hide();

  
  
  
  virtual void SetSlideDuration(int duration);
  int GetSlideDuration() const { return slide_duration_; }
  void SetTweenType(Tween::Type tween_type) { tween_type_ = tween_type; }

  virtual double GetCurrentValue() const OVERRIDE;
  bool IsShowing() const { return showing_; }
  bool IsClosing() const { return !showing_ && value_end_ < value_current_; }

  class TestApi;

 private:
  
  virtual void AnimateToState(double state) OVERRIDE;

  AnimationDelegate* target_;

  Tween::Type tween_type_;

  
  bool showing_;

  
  
  double value_start_;
  double value_end_;
  double value_current_;

  
  
  int slide_duration_;

  DISALLOW_COPY_AND_ASSIGN(SlideAnimation);
};

}  

#endif  
