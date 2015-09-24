// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_GESTURES_SYSTEM_PINCH_HANDLER_H_
#define ASH_WM_GESTURES_SYSTEM_PINCH_HANDLER_H_

#include "ash/wm/workspace/phantom_window_controller.h"

namespace aura {
class Window;
}

namespace gfx {
class Point;
}

namespace ui {
class GestureEvent;
}

namespace views {
class Widget;
}

namespace ash {
namespace internal {

enum SystemGestureStatus {
  SYSTEM_GESTURE_PROCESSED,  
  SYSTEM_GESTURE_IGNORED,    
  SYSTEM_GESTURE_END,        
};

class SystemPinchHandler {
 public:
  explicit SystemPinchHandler(aura::Window* target);
  virtual ~SystemPinchHandler();

  
  
  
  
  SystemGestureStatus ProcessGestureEvent(const ui::GestureEvent& event);

  static const int kSystemGesturePoints;

 private:
  
  
  
  gfx::Rect GetPhantomWindowScreenBounds(aura::Window* window,
                                         const gfx::Point& point);

  enum PhantomWindowState {
    PHANTOM_WINDOW_NORMAL,
    PHANTOM_WINDOW_MAXIMIZED,
    PHANTOM_WINDOW_MINIMIZED,
  };

  aura::Window* target_;
  views::Widget* widget_;

  
  
  PhantomWindowController phantom_;

  
  
  
  PhantomWindowState phantom_state_;

  
  
  
  double pinch_factor_;

  DISALLOW_COPY_AND_ASSIGN(SystemPinchHandler);
};

}  
}  

#endif  
