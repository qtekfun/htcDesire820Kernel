// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_ACCESSIBILITY_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_ACCESSIBILITY_HELPER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "chrome/browser/chromeos/login/wizard_accessibility_handler.h"
#include "content/common/notification_registrar.h"
#include "ui/base/keycodes/keyboard_codes.h"

class Profile;
namespace views {
class Accelerator;
class View;
}

namespace chromeos {

class WizardAccessibilityHelper {
 public:
  
  static WizardAccessibilityHelper* GetInstance();

  
  static views::Accelerator GetAccelerator();

  
  
  
  
  
  void MaybeSpeak(const char* str, bool queue, bool interruptible);

  
  void UnregisterNotifications();

  
  void ToggleAccessibility();

  
  void Init();

 private:
  friend struct DefaultSingletonTraits<WizardAccessibilityHelper>;

  WizardAccessibilityHelper();

  virtual ~WizardAccessibilityHelper() {}

  void RegisterNotifications();

  bool IsAccessibilityEnabled();

  void SetAccessibilityEnabled(bool);

  static scoped_ptr<views::Accelerator> accelerator_;

  scoped_ptr<WizardAccessibilityHandler> accessibility_handler_;

  Profile* profile_;

  
  NotificationRegistrar registrar_;

  bool registered_notifications_;

  DISALLOW_COPY_AND_ASSIGN(WizardAccessibilityHelper);
};

}  

#endif  
