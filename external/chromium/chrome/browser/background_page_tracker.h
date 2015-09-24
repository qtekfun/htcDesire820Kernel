// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_PAGE_TRACKER_H_
#define CHROME_BROWSER_BACKGROUND_PAGE_TRACKER_H_
#pragma once

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/singleton.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class PrefService;

class BackgroundPageTracker : public NotificationObserver {
 public:
  static void RegisterPrefs(PrefService* prefs);

  
  static BackgroundPageTracker* GetInstance();

  
  int GetBackgroundPageCount();

  
  
  int GetUnacknowledgedBackgroundPageCount();

  
  
  
  void AcknowledgeBackgroundPages();

 protected:
  
  BackgroundPageTracker();
  ~BackgroundPageTracker();
  friend struct DefaultSingletonTraits<BackgroundPageTracker>;
  friend class BackgroundPageTrackerTest;
  FRIEND_TEST_ALL_PREFIXES(BackgroundPageTrackerTest, OnBackgroundPageLoaded);
  FRIEND_TEST_ALL_PREFIXES(BackgroundPageTrackerTest,
                           AcknowledgeBackgroundPages);
  FRIEND_TEST_ALL_PREFIXES(BackgroundPageTrackerTest,
                           TestTrackerChangedNotifications);

  
  
  virtual PrefService* GetPrefService();

  
  
  
  virtual bool IsEnabled();

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  bool UpdateExtensionList();

  
  
  
  
  void OnBackgroundPageLoaded(const std::string& parent_application_id);

  
  
  
  void OnExtensionUnloaded(const std::string& parent_application_id);

  
  void SendChangeNotification();

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundPageTracker);
};

#endif  
