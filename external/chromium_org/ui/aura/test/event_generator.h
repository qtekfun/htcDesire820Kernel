// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_EVENT_GENERATOR_H_
#define UI_AURA_TEST_EVENT_GENERATOR_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "ui/events/event_constants.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/point.h"

namespace base {
class TimeDelta;
}

namespace ui {
class Event;
class KeyEvent;
class MouseEvent;
class ScrollEvent;
class TouchEvent;
}

namespace aura {
class RootWindow;
class Window;

namespace client {
class ScreenPositionClient;
}

namespace test {

typedef base::Callback<void(ui::EventType, const gfx::Vector2dF&)>
        ScrollStepCallback;

class EventGeneratorDelegate {
 public:
  virtual ~EventGeneratorDelegate() {}

  
  virtual RootWindow* GetRootWindowAt(const gfx::Point& point) const = 0;

  
  
  
  virtual client::ScreenPositionClient* GetScreenPositionClient(
      const aura::Window* window) const = 0;
};


class EventGenerator {
 public:
  
  
  explicit EventGenerator(Window* root_window);

  
  
  explicit EventGenerator(EventGeneratorDelegate* delegate);

  
  
  EventGenerator(Window* root_window, const gfx::Point& initial_location);

  
  
  EventGenerator(Window* root_window, Window* window);

  virtual ~EventGenerator();

  
  
  
  void set_current_location(const gfx::Point& location) {
    current_location_ = location;
  }
  const gfx::Point& current_location() const { return current_location_; }

  void set_async(bool async) { async_ = async; }
  bool async() const { return async_; }

  
  void set_flags(int flags) { flags_ = flags; }

  
  void PressLeftButton();

  
  void ReleaseLeftButton();

  
  void ClickLeftButton();

  
  void DoubleClickLeftButton();

  
  void PressRightButton();

  
  void ReleaseRightButton();

  
  void SendMouseExit();

  
  
  void MoveMouseToInHost(const gfx::Point& point_in_host);
  void MoveMouseToInHost(int x, int y) {
    MoveMouseToInHost(gfx::Point(x, y));
  }

  
  
  void MoveMouseTo(const gfx::Point& point_in_screen, int count);
  void MoveMouseTo(const gfx::Point& point_in_screen) {
    MoveMouseTo(point_in_screen, 1);
  }
  void MoveMouseTo(int x, int y) {
    MoveMouseTo(gfx::Point(x, y));
  }

  
  
  void MoveMouseRelativeTo(const Window* window, const gfx::Point& point);
  void MoveMouseRelativeTo(const Window* window, int x, int y) {
    MoveMouseRelativeTo(window, gfx::Point(x, y));
  }

  void MoveMouseBy(int x, int y) {
    MoveMouseTo(current_location_ + gfx::Vector2d(x, y));
  }

  
  void DragMouseTo(const gfx::Point& point);

  void DragMouseTo(int x, int y) {
    DragMouseTo(gfx::Point(x, y));
  }

  void DragMouseBy(int dx, int dy) {
    DragMouseTo(current_location_ + gfx::Vector2d(dx, dy));
  }

  
  void MoveMouseToCenterOf(Window* window);

  
  void PressTouch();

  
  void PressTouchId(int touch_id);

  
  void MoveTouch(const gfx::Point& point);

  
  void MoveTouchId(const gfx::Point& point, int touch_id);

  
  void ReleaseTouch();

  
  void ReleaseTouchId(int touch_id);

  
  
  void PressMoveAndReleaseTouchTo(const gfx::Point& point);

  void PressMoveAndReleaseTouchTo(int x, int y) {
    PressMoveAndReleaseTouchTo(gfx::Point(x, y));
  }

  void PressMoveAndReleaseTouchBy(int x, int y) {
    PressMoveAndReleaseTouchTo(current_location_ + gfx::Vector2d(x, y));
  }

  
  
  void PressMoveAndReleaseTouchToCenterOf(Window* window);

  
  
  
  void GestureTapAt(const gfx::Point& point);

  
  
  
  void GestureTapDownAndUp(const gfx::Point& point);

  
  
  
  
  
  void GestureScrollSequence(const gfx::Point& start,
                             const gfx::Point& end,
                             const base::TimeDelta& duration,
                             int steps);

  
  
  
  
  void GestureScrollSequenceWithCallback(const gfx::Point& start,
                                         const gfx::Point& end,
                                         const base::TimeDelta& duration,
                                         int steps,
                                         const ScrollStepCallback& callback);

  
  
  
  
  
  
  
  
  
  void GestureMultiFingerScroll(int count,
                                const gfx::Point start[],
                                int event_separation_time_ms,
                                int steps,
                                int move_x,
                                int move_y);

  
  
  
  
  
  
  
  
  
  
  void GestureMultiFingerScrollWithDelays(int count,
                                          const gfx::Point start[],
                                          const int delay_adding_finger_ms[],
                                          int event_separation_time_ms,
                                          int steps,
                                          int move_x,
                                          int move_y);

  
  
  void ScrollSequence(const gfx::Point& start,
                      const base::TimeDelta& step_delay,
                      float x_offset,
                      float y_offset,
                      int steps,
                      int num_fingers);

  
  
  void ScrollSequence(const gfx::Point& start,
                      const base::TimeDelta& step_delay,
                      const std::vector<gfx::Point>& offsets,
                      int num_fingers);

  
  
  
  
  void PressKey(ui::KeyboardCode key_code, int flags);

  
  
  
  
  void ReleaseKey(ui::KeyboardCode key_code, int flags);

  
  void Dispatch(ui::Event* event);

  void set_current_root_window(RootWindow* root_window) {
    current_root_window_ = root_window;
  }

 private:
  
  void DispatchKeyEvent(bool is_press, ui::KeyboardCode key_code, int flags);

  void UpdateCurrentRootWindow(const gfx::Point& point);
  void PressButton(int flag);
  void ReleaseButton(int flag);

  
  
  void ConvertPointFromTarget(const aura::Window* target,
                              gfx::Point* point) const;
  void ConvertPointToTarget(const aura::Window* target,
                            gfx::Point* point) const;

  gfx::Point GetLocationInCurrentRoot() const;
  gfx::Point CenterOfWindow(const Window* window) const;

  void DispatchNextPendingEvent();
  void DoDispatchEvent(ui::Event* event, bool async);

  scoped_ptr<EventGeneratorDelegate> delegate_;
  gfx::Point current_location_;
  RootWindow* current_root_window_;
  int flags_;
  bool grab_;
  std::list<ui::Event*> pending_events_;
  
  bool async_;

  DISALLOW_COPY_AND_ASSIGN(EventGenerator);
};

}  
}  

#endif  
