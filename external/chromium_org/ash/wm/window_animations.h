// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_ANIMATIONS_H_
#define ASH_WM_WINDOW_ANIMATIONS_H_

#include "ash/ash_export.h"
#include "ui/gfx/transform.h"
#include "ui/views/corewm/window_animations.h"

namespace aura {
class Window;
}
namespace ui {
class Layer;
}

namespace ash {

enum WindowVisibilityAnimationType {
  
  WINDOW_VISIBILITY_ANIMATION_TYPE_MINIMIZE =
      views::corewm::WINDOW_VISIBILITY_ANIMATION_MAX,
  
  WINDOW_VISIBILITY_ANIMATION_TYPE_BRIGHTNESS_GRAYSCALE
};

enum LayerScaleAnimationDirection {
  LAYER_SCALE_ANIMATION_ABOVE,
  LAYER_SCALE_ANIMATION_BELOW,
};

extern const int kCrossFadeDurationMS;

ASH_EXPORT void CrossFadeToBounds(aura::Window* window,
                                  const gfx::Rect& new_bounds);

ASH_EXPORT base::TimeDelta GetCrossFadeDuration(aura::Window* window,
                                                const gfx::Rect& old_bounds,
                                                const gfx::Rect& new_bounds);

ASH_EXPORT bool AnimateOnChildWindowVisibilityChanged(aura::Window* window,
                                                      bool visible);

ASH_EXPORT std::vector<ui::LayerAnimationSequence*>
CreateBrightnessGrayscaleAnimationSequence(float target_value,
                                           base::TimeDelta duration);

ASH_EXPORT void SetTransformForScaleAnimation(
    ui::Layer* layer,
    LayerScaleAnimationDirection type);

ASH_EXPORT gfx::Rect GetMinimizeAnimationTargetBoundsInScreen(
    aura::Window* window);

}  

#endif  
