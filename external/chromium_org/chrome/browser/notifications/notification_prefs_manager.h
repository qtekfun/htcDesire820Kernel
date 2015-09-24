// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_PREFS_MANAGER_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_PREFS_MANAGER_H_

#include "chrome/browser/notifications/balloon_collection.h"

class PrefService;
class PrefRegistrySimple;

class NotificationPrefsManager {
 public:
  explicit NotificationPrefsManager(PrefService* prefs);
  virtual ~NotificationPrefsManager() {}

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  virtual BalloonCollection::PositionPreference
      GetPositionPreference() const = 0;

  
  
  virtual void SetPositionPreference(
      BalloonCollection::PositionPreference preference) = 0;

 protected:
  NotificationPrefsManager() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(NotificationPrefsManager);
};

#endif  
