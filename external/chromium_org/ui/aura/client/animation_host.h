// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_ANIMATION_HOST_H_
#define UI_AURA_CLIENT_ANIMATION_HOST_H_

#include "base/compiler_specific.h"
#include "ui/aura/aura_export.h"

namespace gfx {
class Vector2d;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT AnimationHost {
 public:
  
  
  
  
  
  
  virtual void SetHostTransitionOffsets(
      const gfx::Vector2d& top_left_delta,
      const gfx::Vector2d& bottom_right_delta) = 0;

  
  virtual void OnWindowHidingAnimationCompleted() = 0;

 protected:
  virtual ~AnimationHost() {}
};

AURA_EXPORT void SetAnimationHost(Window* window,
                                  AnimationHost* animation_host);
AURA_EXPORT AnimationHost* GetAnimationHost(Window* window);

}  
}  

#endif  
