// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_PINNED_TAB_SERVICE_H_
#define CHROME_BROWSER_TABS_PINNED_TAB_SERVICE_H_
#pragma once

#include "chrome/browser/profiles/profile_keyed_service.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;

class PinnedTabService : public NotificationObserver,
                         public ProfileKeyedService {
 public:
  explicit PinnedTabService(Profile* profile);

 private:
  
  void GotExit();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  Profile* profile_;

  
  
  bool got_exiting_;

  
  bool has_normal_browser_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PinnedTabService);
};

#endif  
