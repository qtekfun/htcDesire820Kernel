// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_ANIMATION_CONTAINER_OBSERVER_H_
#define UI_GFX_ANIMATION_ANIMATION_CONTAINER_OBSERVER_H_

#include "ui/gfx/gfx_export.h"

namespace gfx {

class AnimationContainer;

class GFX_EXPORT AnimationContainerObserver {
 public:
  
  
  virtual void AnimationContainerProgressed(
      AnimationContainer* container) = 0;

  
  virtual void AnimationContainerEmpty(AnimationContainer* container) = 0;

 protected:
  virtual ~AnimationContainerObserver() {}
};

}  

#endif  
