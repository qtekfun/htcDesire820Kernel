// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELL_OBSERVER_H_
#define ASH_SHELL_OBSERVER_H_

#include "ash/ash_export.h"
#include "ash/system/user/login_status.h"

namespace aura {
class Window;
}

namespace ash {

class ASH_EXPORT ShellObserver {
 public:
  
  virtual void OnDisplayWorkAreaInsetsChanged() {}

  
  virtual void OnLoginStateChanged(user::LoginStatus status) {}

  
  virtual void OnAppTerminating() {}

  
  
  virtual void OnLockStateChanged(bool locked) {}

  
  virtual void OnShelfAlignmentChanged(aura::Window* root_window) {}

  
  virtual void OnTouchHudProjectionToggled(bool enabled) {}

  
  virtual void OnFullscreenStateChanged(bool is_fullscreen,
                                        aura::Window* root_window) {}

 protected:
  virtual ~ShellObserver() {}
};

}  

#endif  
