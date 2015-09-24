// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_POWER_BUTTON_CONTROLLER_H_
#define ASH_WM_POWER_BUTTON_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/wm/session_state_animator.h"
#include "base/basictypes.h"

namespace gfx {
class Rect;
class Size;
}

namespace ui {
class Layer;
}

namespace ash {

namespace test {
class PowerButtonControllerTest;
}

class LockStateController;

class ASH_EXPORT PowerButtonController {
 public:

  explicit PowerButtonController(LockStateController* controller);
  virtual ~PowerButtonController();

  void set_has_legacy_power_button_for_test(bool legacy) {
    has_legacy_power_button_ = legacy;
  }

  
  void OnScreenBrightnessChanged(double percent);

  
  void OnPowerButtonEvent(bool down, const base::TimeTicks& timestamp);
  void OnLockButtonEvent(bool down, const base::TimeTicks& timestamp);

 private:
  friend class test::PowerButtonControllerTest;

  
  bool power_button_down_;
  bool lock_button_down_;

  
  bool screen_is_off_;

  
  
  bool has_legacy_power_button_;

  LockStateController* controller_; 

  DISALLOW_COPY_AND_ASSIGN(PowerButtonController);
};

}  

#endif  
