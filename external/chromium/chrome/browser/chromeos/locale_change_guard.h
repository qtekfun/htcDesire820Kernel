// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOCALE_CHANGE_GUARD_H_
#define CHROME_BROWSER_CHROMEOS_LOCALE_CHANGE_GUARD_H_
#pragma once

#include <string>

#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/notifications/system_notification.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"

class ListValue;
class NotificationDetails;
class NotificationSource;
class Profile;

namespace chromeos {

class LocaleChangeGuard : public NotificationObserver {
 public:
  explicit LocaleChangeGuard(Profile* profile);

  
  void PrepareChangingLocale(
      const std::string& from_locale, const std::string& to_locale);

  
  void OnLogin();

 private:
  class Delegate;

  void RevertLocaleChange(const ListValue* list);
  void AcceptLocaleChange();
  void Check();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  std::string from_locale_;
  std::string to_locale_;
  Profile* profile_;
  scoped_ptr<chromeos::SystemNotification> note_;
  bool reverted_;
  NotificationRegistrar registrar_;

  
  
  
  string16 title_text_;
  string16 message_text_;
  string16 revert_link_text_;
};

}  

#endif  
