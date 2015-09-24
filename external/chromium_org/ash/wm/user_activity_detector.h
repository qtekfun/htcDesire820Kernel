// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_USER_ACTIVITY_DETECTOR_H_
#define ASH_WM_USER_ACTIVITY_DETECTOR_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "ui/events/event_handler.h"

namespace ash {

class UserActivityObserver;

class ASH_EXPORT UserActivityDetector : public ui::EventHandler {
 public:
  
  static const int kNotifyIntervalMs;

  
  
  static const int kDisplayPowerChangeIgnoreMouseMs;

  UserActivityDetector();
  virtual ~UserActivityDetector();

  base::TimeTicks last_activity_time() const { return last_activity_time_; }

  void set_now_for_test(base::TimeTicks now) { now_for_test_ = now; }

  bool HasObserver(UserActivityObserver* observer) const;
  void AddObserver(UserActivityObserver* observer);
  void RemoveObserver(UserActivityObserver* observer);

  
  void OnDisplayPowerChanging();

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  
  base::TimeTicks GetCurrentTime() const;

  
  
  
  void HandleActivity(const ui::Event* event);

  ObserverList<UserActivityObserver> observers_;

  
  base::TimeTicks last_activity_time_;

  
  base::TimeTicks last_observer_notification_time_;

  
  
  base::TimeTicks now_for_test_;

  
  
  
  base::TimeTicks honor_mouse_events_time_;

  DISALLOW_COPY_AND_ASSIGN(UserActivityDetector);
};

}  

#endif  
