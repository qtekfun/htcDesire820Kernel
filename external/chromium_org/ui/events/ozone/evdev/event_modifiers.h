// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_OZONE_EVDEV_EVENT_MODIFIERS_H_
#define UI_EVENTS_OZONE_EVDEV_EVENT_MODIFIERS_H_

#include "ui/events/events_export.h"
#include "ui/events/ozone/event_converter_ozone.h"

namespace ui {

enum {
  EVDEV_MODIFIER_NONE,
  EVDEV_MODIFIER_CAPS_LOCK,
  EVDEV_MODIFIER_SHIFT,
  EVDEV_MODIFIER_CONTROL,
  EVDEV_MODIFIER_ALT,
  EVDEV_MODIFIER_LEFT_MOUSE_BUTTON,
  EVDEV_MODIFIER_MIDDLE_MOUSE_BUTTON,
  EVDEV_MODIFIER_RIGHT_MOUSE_BUTTON,
  EVDEV_MODIFIER_COMMAND,
  EVDEV_MODIFIER_ALTGR,
  EVDEV_NUM_MODIFIERS
};

class EVENTS_EXPORT EventModifiersEvdev {
 public:
  EventModifiersEvdev();
  ~EventModifiersEvdev();

  
  void UpdateModifier(unsigned int modifier, bool down);

  
  void UpdateModifierLock(unsigned int modifier, bool down);

  
  int GetModifierFlags();

 private:
  
  int modifiers_down_[EVDEV_NUM_MODIFIERS];

  
  int modifier_flags_locked_;

  
  int modifier_flags_;

  
  void UpdateFlags(unsigned int modifier);

  DISALLOW_COPY_AND_ASSIGN(EventModifiersEvdev);
};

}  

#endif  
