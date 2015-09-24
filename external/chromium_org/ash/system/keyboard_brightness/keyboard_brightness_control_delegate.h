// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_KEYBOARD_BRIGHTNESS_KEYBOARD_BRIGHTNESS_CONTROL_DELEGATE_H_
#define ASH_SYSTEM_KEYBOARD_BRIGHTNESS_KEYBOARD_BRIGHTNESS_CONTROL_DELEGATE_H_

namespace ui {
class Accelerator;
}  

namespace ash {

class KeyboardBrightnessControlDelegate {
 public:
  virtual ~KeyboardBrightnessControlDelegate() {}

  
  
  virtual bool HandleKeyboardBrightnessDown(
      const ui::Accelerator& accelerator) = 0;
  virtual bool HandleKeyboardBrightnessUp(
      const ui::Accelerator& accelerator) = 0;
};

}  

#endif  
