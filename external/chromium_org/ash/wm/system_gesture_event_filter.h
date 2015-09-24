// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_SYSTEM_GESTURE_EVENT_FILTER_H_
#define ASH_WM_SYSTEM_GESTURE_EVENT_FILTER_H_

#include "ash/shell.h"
#include "ash/touch/touch_uma.h"
#include "base/timer/timer.h"
#include "ui/aura/window_observer.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/point.h"

#include <map>

namespace aura {
class Window;
}

namespace ui {
class LocatedEvent;
}

namespace ash {

namespace test {
class SystemGestureEventFilterTest;
}

namespace internal {
class LongPressAffordanceHandler;
class OverviewGestureHandler;
class SystemPinchHandler;
class TouchUMA;
class TwoFingerDragHandler;

class SystemGestureEventFilter : public ui::EventHandler,
                                 public aura::WindowObserver {
 public:
  SystemGestureEventFilter();
  virtual ~SystemGestureEventFilter();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

 private:
  friend class ash::test::SystemGestureEventFilterTest;

  
  void ClearGestureHandlerForWindow(aura::Window* window);

  typedef std::map<aura::Window*, SystemPinchHandler*> WindowPinchHandlerMap;
  
  
  WindowPinchHandlerMap pinch_handlers_;

  bool system_gestures_enabled_;

  scoped_ptr<LongPressAffordanceHandler> long_press_affordance_;
  scoped_ptr<OverviewGestureHandler> overview_gesture_handler_;
  scoped_ptr<TwoFingerDragHandler> two_finger_drag_;

  DISALLOW_COPY_AND_ASSIGN(SystemGestureEventFilter);
};

}  
}  

#endif  
