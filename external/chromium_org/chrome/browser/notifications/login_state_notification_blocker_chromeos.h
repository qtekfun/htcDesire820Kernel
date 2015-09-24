// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_LOGIN_STATE_NOTIFICATION_BLOCKER_CHROMEOS_H_
#define CHROME_BROWSER_NOTIFICATIONS_LOGIN_STATE_NOTIFICATION_BLOCKER_CHROMEOS_H_

#include "ash/shell_observer.h"
#include "chrome/browser/chromeos/login/user_adding_screen.h"
#include "chromeos/login/login_state.h"
#include "ui/message_center/notification_blocker.h"

class LoginStateNotificationBlockerChromeOS
    : public message_center::NotificationBlocker,
      public ash::ShellObserver,
      public chromeos::LoginState::Observer,
      public chromeos::UserAddingScreen::Observer {
 public:
  explicit LoginStateNotificationBlockerChromeOS(
      message_center::MessageCenter* message_center);
  virtual ~LoginStateNotificationBlockerChromeOS();

 private:
  
  virtual bool ShouldShowNotificationAsPopup(
      const message_center::NotifierId& notifier_id) const OVERRIDE;

  
  virtual void OnLockStateChanged(bool locked) OVERRIDE;
  virtual void OnAppTerminating() OVERRIDE;

  
  virtual void LoggedInStateChanged() OVERRIDE;

  
  virtual void OnUserAddingStarted() OVERRIDE;
  virtual void OnUserAddingFinished() OVERRIDE;

  bool locked_;
  bool observing_;

  DISALLOW_COPY_AND_ASSIGN(LoginStateNotificationBlockerChromeOS);
};

#endif  
