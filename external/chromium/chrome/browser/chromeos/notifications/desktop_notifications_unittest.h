// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_DESKTOP_NOTIFICATIONS_UNITTEST_H_
#define CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_DESKTOP_NOTIFICATIONS_UNITTEST_H_
#pragma once

#include <set>
#include <string>

#include "base/message_loop.h"
#include "base/string_util.h"
#include "chrome/browser/chromeos/notifications/balloon_collection_impl.h"
#include "chrome/browser/notifications/balloon.h"
#include "chrome/browser/notifications/desktop_notification_service.h"
#include "chrome/browser/notifications/notification.h"
#include "chrome/browser/notifications/notification_test_util.h"
#include "chrome/browser/notifications/notification_ui_manager.h"
#include "chrome/browser/notifications/notifications_prefs_cache.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/test/testing_pref_service.h"
#include "chrome/test/testing_profile.h"
#include "content/browser/browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

struct DesktopNotificationHostMsg_Show_Params;

namespace chromeos {

class DesktopNotificationsTest;
typedef LoggingNotificationDelegate<DesktopNotificationsTest>
    LoggingNotificationProxy;

class MockBalloonCollection : public BalloonCollectionImpl {
 public:
  MockBalloonCollection();

  
  virtual void Add(const Notification& notification,
                   Profile* profile);
  virtual Balloon* MakeBalloon(const Notification& notification,
                               Profile* profile);
  virtual void OnBalloonClosed(Balloon* source);

  
  std::set<Balloon*>& balloons() { return balloons_; }
  int count() const { return balloons_.size(); }

  
  int UppermostVerticalPosition();

 private:
  std::set<Balloon*> balloons_;
};

class DesktopNotificationsTest : public testing::Test {
 public:
  DesktopNotificationsTest();
  ~DesktopNotificationsTest();

  static void log(const std::string& message) {
    log_output_.append(message);
  }

  Profile* profile() { return profile_.get(); }

 protected:
  
  virtual void SetUp();
  virtual void TearDown();

  void AllowOrigin(const GURL& origin) {
    service_->GrantPermission(origin);
  }

  void DenyOrigin(const GURL& origin) {
    service_->DenyPermission(origin);
  }

  int HasPermission(const GURL& origin) {
    return service_->prefs_cache()->HasPermission(origin);
  }

  
  DesktopNotificationHostMsg_Show_Params StandardTestNotification();

  
  
  MessageLoopForUI message_loop_;
  BrowserThread ui_thread_;

  
  TestingPrefService local_state_;

  
  scoped_ptr<TestingProfile> profile_;

  
  MockBalloonCollection* balloon_collection_;

  
  scoped_ptr<NotificationUIManager> ui_manager_;

  
  scoped_ptr<DesktopNotificationService> service_;

  
  static std::string log_output_;
};

}  

#endif  
