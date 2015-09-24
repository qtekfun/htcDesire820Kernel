// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_OZONE_EVDEV_TOUCH_EVENT_CONVERTER_EVDEV_H_
#define UI_EVENTS_OZONE_EVDEV_TOUCH_EVENT_CONVERTER_EVDEV_H_

#include <bitset>

#include "base/compiler_specific.h"
#include "ui/events/event_constants.h"
#include "ui/events/events_export.h"
#include "ui/events/ozone/event_converter_ozone.h"

namespace ui {

class TouchEvent;

class EVENTS_EXPORT TouchEventConverterEvdev : public EventConverterOzone {
 public:
  enum {
    MAX_FINGERS = 11
  };
  TouchEventConverterEvdev(int fd, int id);
  virtual ~TouchEventConverterEvdev();

 private:
  friend class MockTouchEventConverterEvdev;

  
  void Init();

  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;

  
  int pressure_min_;
  int pressure_max_;  

  
  float x_scale_;
  float y_scale_;

  
  int x_max_;
  int y_max_;

  
  int current_slot_;

  
  int fd_;

  
  int id_;

  
  
  std::bitset<MAX_FINGERS> altered_slots_;

  struct InProgressEvents {
    int x_;
    int y_;
    int id_;  
    int finger_;  

    EventType type_;
    int major_;
    float pressure_;
  };

  
  InProgressEvents events_[MAX_FINGERS];

  DISALLOW_COPY_AND_ASSIGN(TouchEventConverterEvdev);
};

}  

#endif  

