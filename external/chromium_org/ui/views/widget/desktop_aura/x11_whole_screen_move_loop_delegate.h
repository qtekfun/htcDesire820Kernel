// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WHOLE_SCREEN_MOVE_LOOP_DELEGATE_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WHOLE_SCREEN_MOVE_LOOP_DELEGATE_H_

#include <X11/Xlib.h>
#undef RootWindow

namespace views {

class X11WholeScreenMoveLoopDelegate {
 public:
  
  virtual void OnMouseMovement(XMotionEvent* event) = 0;

  
  virtual void OnMouseReleased() = 0;

  
  
  virtual void OnMoveLoopEnded() = 0;
};

}  

#endif  
