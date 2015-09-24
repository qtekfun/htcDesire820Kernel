// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_CONTROLLER_OBSERVER_H_
#define UI_KEYBOARD_KEYBOARD_CONTROLLER_OBSERVER_H_

#include "ui/keyboard/keyboard_export.h"

namespace gfx {
class Rect;
}

namespace keyboard {

class KEYBOARD_EXPORT KeyboardControllerObserver {
 public:
  
  virtual void OnKeyboardBoundsChanging(const gfx::Rect& new_bounds) = 0;
};

}  

#endif  
