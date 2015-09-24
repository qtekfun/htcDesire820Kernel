// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_KEYBOARD_UMA_EVENT_FILTER_H_
#define ASH_KEYBOARD_UMA_EVENT_FILTER_H_

#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "ui/events/event_handler.h"

namespace ash {
namespace internal {

class KeyboardUMAEventFilter : public ui::EventHandler {
 public:
  KeyboardUMAEventFilter();
  virtual ~KeyboardUMAEventFilter();

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;

 private:
  
  base::TimeDelta last_keystroke_time_;

  DISALLOW_COPY_AND_ASSIGN(KeyboardUMAEventFilter);
};

}  
}  

#endif  
