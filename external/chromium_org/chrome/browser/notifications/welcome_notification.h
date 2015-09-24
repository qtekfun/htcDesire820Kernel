// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_WELCOME_NOTIFICATION_H_
#define CHROME_BROWSER_NOTIFICATIONS_WELCOME_NOTIFICATION_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/prefs/pref_service_syncable_observer.h"
#include "ui/message_center/notifier_settings.h"

namespace message_center {
class MessageCenter;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class Notification;
class Profile;

class WelcomeNotification
    : public PrefServiceSyncableObserver {
 public:
  WelcomeNotification(
      Profile* profile,
      message_center::MessageCenter* message_center);
  virtual ~WelcomeNotification();

  
  virtual void OnIsSyncingChanged() OVERRIDE;

  
  
  void ShowWelcomeNotificationIfNecessary(
      const Notification& notification);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* prefs);

 private:

  enum PopUpRequest {
    POP_UP_HIDDEN = 0,
    POP_UP_SHOWN = 1,
  };

  
  void ShowWelcomeNotification(
      const message_center::NotifierId notifier_id,
      const base::string16& display_source,
      PopUpRequest pop_up_request);

  
  void HideWelcomeNotification();

  
  void OnWelcomeNotificationDismissedChanged();

  
  BooleanPrefMember welcome_notification_dismissed_pref_;

  
  Profile* profile_;

  
  std::string welcome_notification_id_;

  
  
  
  
  
  scoped_ptr<Notification> delayed_notification_;

  message_center::MessageCenter* message_center_;  
};

#endif  
