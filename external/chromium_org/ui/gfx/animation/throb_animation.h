// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_THROB_ANIMATION_H_
#define UI_GFX_ANIMATION_THROB_ANIMATION_H_

#include "ui/gfx/animation/slide_animation.h"

namespace gfx {

class GFX_EXPORT ThrobAnimation : public SlideAnimation {
 public:
  explicit ThrobAnimation(AnimationDelegate* target);
  virtual ~ThrobAnimation() {}

  
  
  void StartThrobbing(int cycles_til_stop);

  
  void SetThrobDuration(int duration) { throb_duration_ = duration; }

  
  virtual void Reset() OVERRIDE;
  virtual void Reset(double value) OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;

  
  virtual void SetSlideDuration(int duration) OVERRIDE;

  
  void set_cycles_remaining(int value) { cycles_remaining_ = value; }
  int cycles_remaining() const { return cycles_remaining_; }

 protected:
  
  virtual void Step(base::TimeTicks time_now) OVERRIDE;

 private:
  
  void ResetForSlide();

  
  int slide_duration_;

  
  int throb_duration_;

  
  int cycles_remaining_;

  
  bool throbbing_;

  DISALLOW_COPY_AND_ASSIGN(ThrobAnimation);
};

}  

#endif  
