// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_KEYBOARD_EVENT_COUNTER_H_
#define MEDIA_BASE_KEYBOARD_EVENT_COUNTER_H_

#include <set>

#include "base/synchronization/lock.h"
#include "media/base/media_export.h"
#include "ui/events/event_constants.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace media {

class MEDIA_EXPORT KeyboardEventCounter {
 public:
  KeyboardEventCounter();
  ~KeyboardEventCounter();

  
  
  void Reset();

  
  
  size_t GetKeyPressCount() const;

  
  
  void OnKeyboardEvent(ui::EventType event, ui::KeyboardCode key_code);

 private:
  
  std::set<ui::KeyboardCode> pressed_keys_;

  size_t total_key_presses_;

  DISALLOW_COPY_AND_ASSIGN(KeyboardEventCounter);
};

}  

#endif  
