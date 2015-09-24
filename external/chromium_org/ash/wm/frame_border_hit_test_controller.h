// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_FRAME_BORDER_HITTEST_CONTROLLER_H_
#define ASH_WM_FRAME_BORDER_HITTEST_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/wm/window_state_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/window_observer.h"

namespace gfx {
class Point;
}

namespace views {
class NonClientFrameView;
class Widget;
}

namespace ash {
class HeaderPainter;

class ASH_EXPORT FrameBorderHitTestController : public wm::WindowStateObserver,
                                                public aura::WindowObserver {
 public:
  explicit FrameBorderHitTestController(views::Widget* frame);
  virtual ~FrameBorderHitTestController();

  
  
  static int NonClientHitTest(views::NonClientFrameView* view,
                              HeaderPainter* header_painter,
                              const gfx::Point& point);

 private:
  
  
  void UpdateHitTestBoundsOverrideInner();

  
  virtual void OnWindowShowTypeChanged(wm::WindowState* window_state,
                                       wm::WindowShowType old_type) OVERRIDE;
  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  aura::Window* frame_window_;

  DISALLOW_COPY_AND_ASSIGN(FrameBorderHitTestController);
};

}  

#endif  
