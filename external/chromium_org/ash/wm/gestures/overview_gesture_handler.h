// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_GESTURES_OVERVIEW_GESTURE_HANDLER_H_
#define ASH_WM_GESTURES_OVERVIEW_GESTURE_HANDLER_H_

#include "base/basictypes.h"

namespace aura {
class Window;
}

namespace ui {
class GestureEvent;
class ScrollEvent;
}

namespace ash {
namespace internal {

class OverviewGestureHandler {
 public:
  OverviewGestureHandler();
  virtual ~OverviewGestureHandler();

  
  
  bool ProcessScrollEvent(const ui::ScrollEvent& event);

  
  
  bool ProcessGestureEvent(const ui::GestureEvent& event);

 private:
  
  bool in_top_bezel_gesture_;

  
  float scroll_x_;
  float scroll_y_;

  DISALLOW_COPY_AND_ASSIGN(OverviewGestureHandler);
};

}  
}  

#endif  
