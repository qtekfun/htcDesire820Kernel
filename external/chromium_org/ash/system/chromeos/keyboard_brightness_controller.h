// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_KEYBOARD_BRIGHTNESS_CONTROLLER_H_
#define ASH_SYSTEM_CHROMEOS_KEYBOARD_BRIGHTNESS_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/system/keyboard_brightness/keyboard_brightness_control_delegate.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace ash {

class ASH_EXPORT KeyboardBrightnessController
    : public KeyboardBrightnessControlDelegate {
 public:
  KeyboardBrightnessController() {}
  virtual ~KeyboardBrightnessController() {}

 private:
  
  virtual bool HandleKeyboardBrightnessDown(
      const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool HandleKeyboardBrightnessUp(
      const ui::Accelerator& accelerator) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(KeyboardBrightnessController);
};

}  

#endif  
