// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_TEST_LAYER_ANIMATOR_TEST_CONTROLLER_H_
#define UI_COMPOSITOR_TEST_LAYER_ANIMATOR_TEST_CONTROLLER_H_

#include "ui/compositor/layer_animator.h"

namespace ui {

class LayerAnimatorTestController {
 public:
  LayerAnimatorTestController(scoped_refptr<LayerAnimator> animator);

  ~LayerAnimatorTestController();

  LayerAnimator* animator() { return animator_.get(); }

  
  LayerAnimationSequence* GetRunningSequence(
      LayerAnimationElement::AnimatableProperty property);

  
  void StartThreadedAnimationsIfNeeded();

 private:
  scoped_refptr<LayerAnimator> animator_;
};

}  

#endif  
