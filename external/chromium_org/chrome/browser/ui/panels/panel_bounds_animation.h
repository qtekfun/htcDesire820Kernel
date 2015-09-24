// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_BOUNDS_ANIMATION_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_BOUNDS_ANIMATION_H_

#include "ui/gfx/animation/linear_animation.h"

namespace gfx {
class AnimationDelegate;
}
class Panel;

class PanelBoundsAnimation : public gfx::LinearAnimation {
 public:
  PanelBoundsAnimation(gfx::AnimationDelegate* target,
                       Panel* panel,
                       const gfx::Rect& initial_bounds,
                       const gfx::Rect& final_bounds);
  virtual ~PanelBoundsAnimation();
  virtual double GetCurrentValue() const OVERRIDE;

  
  static double ComputeAnimationValue(double progress,
                                      bool for_big_minimize,
                                      double animation_stop_to_show_titlebar);

 protected:
  virtual void AnimateToState(double state) OVERRIDE {}

 private:
  Panel* panel_;  
  bool for_big_minimize_;
  double animation_stop_to_show_titlebar_;

  DISALLOW_COPY_AND_ASSIGN(PanelBoundsAnimation);
};

#endif  
