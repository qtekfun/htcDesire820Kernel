// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_WINDOW_MOVE_CLIENT_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_WINDOW_MOVE_CLIENT_H_

#include <X11/Xlib.h>

#undef RootWindow

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "base/timer/timer.h"
#include "ui/aura/client/window_move_client.h"
#include "ui/gfx/point.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/desktop_aura/x11_whole_screen_move_loop.h"
#include "ui/views/widget/desktop_aura/x11_whole_screen_move_loop_delegate.h"

namespace aura {
class RootWindow;
}

namespace gfx {
class Rect;
}

namespace views {

class VIEWS_EXPORT X11DesktopWindowMoveClient :
      public views::X11WholeScreenMoveLoopDelegate,
      public aura::client::WindowMoveClient {
 public:
  X11DesktopWindowMoveClient();
  virtual ~X11DesktopWindowMoveClient();

  
  virtual void OnMouseMovement(XMotionEvent* event) OVERRIDE;
  virtual void OnMouseReleased() OVERRIDE;
  virtual void OnMoveLoopEnded() OVERRIDE;

  
  virtual aura::client::WindowMoveResult RunMoveLoop(
      aura::Window* window,
      const gfx::Vector2d& drag_offset,
      aura::client::WindowMoveSource move_source) OVERRIDE;
  virtual void EndMoveLoop() OVERRIDE;

 private:
  
  void SetHostBounds(const gfx::Rect& rect);

  X11WholeScreenMoveLoop move_loop_;

  
  
  aura::RootWindow* root_window_;

  
  
  
  gfx::Vector2d window_offset_;

  base::OneShotTimer<X11DesktopWindowMoveClient> window_move_timer_;
};

}  

#endif  
