// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SCREEN_ROTATION_H_
#define ASH_SCREEN_ROTATION_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/layer_animation_element.h"
#include "ui/gfx/point.h"

namespace ui {
class InterpolatedTransform;
class Layer;
}

namespace aura {
class RootWindow;
}

namespace ash {

class ASH_EXPORT ScreenRotation : public ui::LayerAnimationElement {
 public:
  
  
  ScreenRotation(int degrees, ui::Layer* layer);
  virtual ~ScreenRotation();

 private:
  
  
  void InitTransform(ui::Layer* layer);

  
  virtual void OnStart(ui::LayerAnimationDelegate* delegate) OVERRIDE;
  virtual bool OnProgress(double t,
                          ui::LayerAnimationDelegate* delegate) OVERRIDE;
  virtual void OnGetTarget(TargetValue* target) const OVERRIDE;
  virtual void OnAbort(ui::LayerAnimationDelegate* delegate) OVERRIDE;

  static const ui::LayerAnimationElement::AnimatableProperties&
      GetProperties();

  scoped_ptr<ui::InterpolatedTransform> interpolated_transform_;

  
  int degrees_;

  
  gfx::Point new_origin_;

  DISALLOW_COPY_AND_ASSIGN(ScreenRotation);
};

}  

#endif  
