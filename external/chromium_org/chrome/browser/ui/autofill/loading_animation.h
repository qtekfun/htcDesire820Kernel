// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_LOADING_ANIMATION_H_
#define CHROME_BROWSER_UI_AUTOFILL_LOADING_ANIMATION_H_

#include "ui/gfx/animation/linear_animation.h"

namespace autofill {

class LoadingAnimation : public gfx::LinearAnimation {
 public:
  explicit LoadingAnimation(gfx::AnimationDelegate* delegate,
                            int font_height);
  virtual ~LoadingAnimation();

  
  virtual void Step(base::TimeTicks time_now) OVERRIDE;

  
  double GetCurrentValueForDot(size_t dot_i) const;

  
  
  void Reset();

 private:
  
  struct AnimationFrame {
    double value;
    double position;
  };

  
  bool first_cycle_;

  
  
  const int font_height_;
};

}  

#endif  
