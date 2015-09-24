// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_ANIMATION_CONTAINER_ELEMENT_H_
#define UI_GFX_ANIMATION_ANIMATION_CONTAINER_ELEMENT_H_

#include "base/time/time.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT AnimationContainerElement {
 public:
  
  
  virtual void SetStartTime(base::TimeTicks start_time) = 0;

  
  virtual void Step(base::TimeTicks time_now) = 0;

  
  
  virtual base::TimeDelta GetTimerInterval() const = 0;

 protected:
  virtual ~AnimationContainerElement() {}
};

}  

#endif  
