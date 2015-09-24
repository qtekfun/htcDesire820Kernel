// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_OVERSCROLL_CONTROLLER_DELEGATE_H_
#define CONTENT_BROWSER_RENDERER_HOST_OVERSCROLL_CONTROLLER_DELEGATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/browser/renderer_host/overscroll_controller.h"
#include "ui/gfx/rect.h"

namespace content {

class OverscrollControllerDelegate {
 public:
  OverscrollControllerDelegate() {}
  virtual ~OverscrollControllerDelegate() {}

  
  
  virtual gfx::Rect GetVisibleBounds() const = 0;

  
  virtual void OnOverscrollUpdate(float delta_x, float delta_y) = 0;

  
  virtual void OnOverscrollComplete(OverscrollMode overscroll_mode) = 0;

  
  virtual void OnOverscrollModeChange(OverscrollMode old_mode,
                                      OverscrollMode new_mode) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(OverscrollControllerDelegate);
};

}  

#endif  
