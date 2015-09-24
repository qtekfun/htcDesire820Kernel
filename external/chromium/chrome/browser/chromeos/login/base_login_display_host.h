// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_BASE_LOGIN_DISPLAY_HOST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_BASE_LOGIN_DISPLAY_HOST_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "chrome/browser/chromeos/login/login_display_host.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/rect.h"

class WizardController;

namespace views {
class Widget;
}

namespace chromeos {

class ExistingUserController;

class BaseLoginDisplayHost : public LoginDisplayHost,
                             public NotificationObserver {
 public:
  explicit BaseLoginDisplayHost(const gfx::Rect& background_bounds);
  virtual ~BaseLoginDisplayHost();

  
  static LoginDisplayHost* default_host() {
    return default_host_;
  }

  
  virtual void OnSessionStart();
  virtual void StartWizard(
      const std::string& first_screen_name,
      const GURL& start_url);
  virtual void StartSignInScreen();

  const gfx::Rect& background_bounds() const { return background_bounds_; }

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  gfx::Rect background_bounds_;

  NotificationRegistrar registrar_;

  
  static LoginDisplayHost* default_host_;

  
  scoped_ptr<ExistingUserController> sign_in_controller_;

  
  scoped_ptr<WizardController> wizard_controller_;

  DISALLOW_COPY_AND_ASSIGN(BaseLoginDisplayHost);
};

}  

#endif  
