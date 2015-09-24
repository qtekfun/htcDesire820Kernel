// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_X_TOUCH_FACTORY_X11_H_
#define UI_EVENTS_X_TOUCH_FACTORY_X11_H_

#include <bitset>
#include <map>
#include <vector>

#include "base/timer/timer.h"
#include "ui/events/events_base_export.h"
#include "ui/gfx/sequential_id_generator.h"

template <typename T> struct DefaultSingletonTraits;

typedef unsigned long Cursor;
typedef unsigned long Window;
typedef struct _XDisplay Display;
typedef union _XEvent XEvent;

namespace ui {

class EVENTS_BASE_EXPORT TouchFactory {
 private:
  TouchFactory();
  ~TouchFactory();

 public:
  
  static TouchFactory* GetInstance();

  
  static void SetTouchDeviceListFromCommandLine();

  
  void UpdateDeviceList(Display* display);

  
  
  bool ShouldProcessXI2Event(XEvent* xevent);

  
  void SetupXI2ForXWindow(::Window xid);

  
  
  
  void SetTouchDeviceList(const std::vector<unsigned int>& devices);

  
  bool IsTouchDevice(unsigned int deviceid) const;

  
  
  bool IsMultiTouchDevice(unsigned int deviceid) const;

  
  
  
  bool QuerySlotForTrackingID(uint32 tracking_id, int* slot);

  
  
  int GetSlotForTrackingID(uint32 tracking_id);

  
  void ReleaseSlotForTrackingID(uint32 tracking_id);

  
  bool IsTouchDevicePresent();

  
  int GetMaxTouchPoints() const;

  
  
  
  void SetTouchDeviceForTest(const std::vector<unsigned int>& devices);

  
  
  
  void SetPointerDeviceForTest(const std::vector<unsigned int>& devices);

 private:
  
  friend struct DefaultSingletonTraits<TouchFactory>;

  
  
  
  
  
  

  static const int kMaxDeviceNum = 128;

  
  
  std::bitset<kMaxDeviceNum> pointer_device_lookup_;

  
  std::bitset<kMaxDeviceNum> touch_device_lookup_;

  
  bool touch_device_available_;

  
  bool touch_events_disabled_;

  
  
  
  
  
  std::map<int, bool> touch_device_list_;

  
  
  
  
  int max_touch_points_;

  SequentialIDGenerator id_generator_;

  DISALLOW_COPY_AND_ASSIGN(TouchFactory);
};

}  

#endif  
