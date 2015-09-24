// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_ANIMATION_DELEGATE_H_
#define UI_GFX_ANIMATION_ANIMATION_DELEGATE_H_

#include "ui/gfx/gfx_export.h"

namespace gfx {

class Animation;

class GFX_EXPORT AnimationDelegate {
 public:
  
  virtual void AnimationEnded(const Animation* animation) {}

  
  virtual void AnimationProgressed(const Animation* animation) {}

  
  virtual void AnimationCanceled(const Animation* animation) {}

 protected:
  virtual ~AnimationDelegate() {}
};

}  

#endif  
