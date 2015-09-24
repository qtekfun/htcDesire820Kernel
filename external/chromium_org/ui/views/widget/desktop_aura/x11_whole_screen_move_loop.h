// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WHOLE_SCREEN_MOVE_LOOP_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WHOLE_SCREEN_MOVE_LOOP_H_

#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/vector2d_f.h"
#include "ui/views/widget/desktop_aura/x11_whole_screen_move_loop_delegate.h"

typedef struct _XDisplay XDisplay;

namespace aura {
class Window;
}

namespace views {

class Widget;

class X11WholeScreenMoveLoop : public base::MessageLoop::Dispatcher {
 public:
  explicit X11WholeScreenMoveLoop(X11WholeScreenMoveLoopDelegate* delegate);
  virtual ~X11WholeScreenMoveLoop();

  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  
  
  
  bool RunMoveLoop(aura::Window* window, gfx::NativeCursor cursor);

  
  void UpdateCursor(gfx::NativeCursor cursor);

  
  void EndMoveLoop();

  
  void SetDragImage(const gfx::ImageSkia& image, gfx::Vector2dF offset);

 private:
  
  
  bool GrabPointerWithCursor(gfx::NativeCursor cursor);

  
  Window CreateDragInputWindow(XDisplay* display);

  
  void CreateDragImageWindow();

  X11WholeScreenMoveLoopDelegate* delegate_;

  
  bool in_move_loop_;

  
  
  
  ::Window grab_input_window_;

  base::Closure quit_closure_;

  
  
  scoped_ptr<Widget> drag_widget_;
  gfx::ImageSkia drag_image_;
  gfx::Vector2dF drag_offset_;

  DISALLOW_COPY_AND_ASSIGN(X11WholeScreenMoveLoop);
};

}  

#endif  
