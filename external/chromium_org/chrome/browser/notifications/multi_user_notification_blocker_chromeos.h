// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_MULTI_USER_NOTIFICATION_BLOCKER_CHROMEOS_H_
#define CHROME_BROWSER_NOTIFICATIONS_MULTI_USER_NOTIFICATION_BLOCKER_CHROMEOS_H_

#include <map>
#include <string>

#include "ash/shell_observer.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chromeos/login/login_state.h"
#include "ui/message_center/notification_blocker.h"

class MultiUserNotificationBlockerChromeOS
    : public message_center::NotificationBlocker,
      public ash::ShellObserver,
      public chromeos::UserManager::UserSessionStateObserver {
 public:
  explicit MultiUserNotificationBlockerChromeOS(
      message_center::MessageCenter* message_center);
  virtual ~MultiUserNotificationBlockerChromeOS();

  
  virtual bool ShouldShowNotification(
      const message_center::NotifierId& notifier_id) const OVERRIDE;
  virtual bool ShouldShowNotificationAsPopup(
      const message_center::NotifierId& notifier_id) const OVERRIDE;

  
  virtual void OnAppTerminating() OVERRIDE;

  
  virtual void ActiveUserChanged(const chromeos::User* active_user) OVERRIDE;

 private:
  
  bool IsActive() const;

  std::string active_user_id_;
  bool observing_;
  std::map<std::string, bool> quiet_modes_;

  DISALLOW_COPY_AND_ASSIGN(MultiUserNotificationBlockerChromeOS);
};

#endif  
