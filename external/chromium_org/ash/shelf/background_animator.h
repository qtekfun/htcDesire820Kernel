// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_BACKGROUND_ANIMATOR_H_
#define ASH_SHELF_BACKGROUND_ANIMATOR_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"

namespace ash {

enum BackgroundAnimatorChangeType {
  BACKGROUND_CHANGE_ANIMATE,
  BACKGROUND_CHANGE_IMMEDIATE
};

namespace internal {

class ASH_EXPORT BackgroundAnimatorDelegate {
 public:
  virtual void UpdateBackground(int alpha) = 0;

 protected:
  virtual ~BackgroundAnimatorDelegate() {}
};

class ASH_EXPORT BackgroundAnimator : public gfx::AnimationDelegate {
 public:
  BackgroundAnimator(BackgroundAnimatorDelegate* delegate,
                     int min_alpha,
                     int max_alpha);
  virtual ~BackgroundAnimator();

  
  void SetDuration(int time_in_ms);

  
  
  
  void SetPaintsBackground(bool value, BackgroundAnimatorChangeType type);
  bool paints_background() const { return paints_background_; }

  
  int alpha() const { return alpha_; }

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 private:
  BackgroundAnimatorDelegate* delegate_;

  const int min_alpha_;
  const int max_alpha_;

  gfx::SlideAnimation animation_;

  
  bool paints_background_;

  
  int alpha_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundAnimator);
};

}  
}  

#endif  
