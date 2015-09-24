// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_GESTURES_GESTURE_POINT_H_
#define UI_EVENTS_GESTURES_GESTURE_POINT_H_

#include "base/basictypes.h"
#include "ui/events/gestures/velocity_calculator.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

namespace ui {
class TouchEvent;

class GesturePoint {
 public:
  GesturePoint();
  ~GesturePoint();

  
  void Reset();

  void ResetVelocity();

  
  void UpdateForTap();

  
  
  void UpdateForScroll();

  
  void UpdateValues(const TouchEvent& event);

  
  
  bool IsInClickWindow(const TouchEvent& event) const;
  bool IsInDoubleClickWindow(const TouchEvent& event) const;
  bool IsInTripleClickWindow(const TouchEvent& event) const;
  bool IsInFlickWindow(const TouchEvent& event);
  bool IsInHorizontalRailWindow() const;
  bool IsInVerticalRailWindow() const;
  bool IsInsideManhattanSquare(const TouchEvent& event) const;
  bool IsInScrollWindow(const TouchEvent& event) const;
  bool BreaksHorizontalRail();
  bool BreaksVerticalRail();
  bool DidScroll(const TouchEvent& event, int distance) const;

  const gfx::Point& first_touch_position() const {
    return first_touch_position_;
  }

  double last_touch_time() const { return last_touch_time_; }
  const gfx::Point& last_touch_position() const { return last_touch_position_; }
  int x() const { return last_touch_position_.x(); }
  int y() const { return last_touch_position_.y(); }

  
  
  
  
  
  
  void set_point_id(int point_id) { point_id_ = point_id; }
  int point_id() const { return point_id_; }

  void set_touch_id(int touch_id) { touch_id_ = touch_id; }
  int touch_id() const { return touch_id_; }

  bool in_use() const { return point_id_ >= 0; }

  gfx::Vector2d ScrollDelta();

  float XVelocity() { return velocity_calculator_.XVelocity(); }
  float YVelocity() { return velocity_calculator_.YVelocity(); }

  const gfx::Rect& enclosing_rectangle() const { return enclosing_rect_; }

 private:
  

  
  
  bool IsConsistentScrollingActionUnderway() const;
  bool IsInClickTimeWindow() const;
  bool IsInClickAggregateTimeWindow(double before, double after) const;
  bool IsPointInsideManhattanSquare(gfx::Point p1, gfx::Point p2) const;
  bool IsOverMinFlickSpeed();

  
  
  int ScrollVelocityDirection(float v);

  
  
  
  
  
  
  void UpdateEnclosingRectangle(const TouchEvent& event);
  void clear_enclosing_rectangle() { enclosing_rect_ = gfx::Rect(); }

  
  gfx::Point first_touch_position_;
  double first_touch_time_;

  gfx::Point second_last_touch_position_;
  double second_last_touch_time_;

  gfx::Point last_touch_position_;
  double last_touch_time_;

  double second_last_tap_time_;
  gfx::Point second_last_tap_position_;

  double last_tap_time_;
  gfx::Point last_tap_position_;

  VelocityCalculator velocity_calculator_;

  int point_id_;
  int touch_id_;

  
  
  gfx::Rect enclosing_rect_;

  
  gfx::Vector2d same_direction_count_;

  DISALLOW_COPY_AND_ASSIGN(GesturePoint);
};

}  

#endif  