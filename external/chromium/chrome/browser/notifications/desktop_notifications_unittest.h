// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATIONS_UNITTEST_H_
#define CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATIONS_UNITTEST_H_
#pragma once

#include <deque>
#include <string>

#include "base/message_loop.h"
#include "chrome/browser/notifications/balloon_collection_impl.h"
#include "chrome/browser/notifications/desktop_notification_service.h"
#include "chrome/browser/notifications/notification.h"
#include "chrome/browser/notifications/notification_test_util.h"
#include "chrome/browser/notifications/notification_ui_manager.h"
#include "chrome/browser/notifications/notifications_prefs_cache.h"
#include "chrome/test/testing_pref_service.h"
#include "chrome/test/testing_profile.h"
#include "content/browser/browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

class DesktopNotificationsTest;
typedef LoggingNotificationDelegate<DesktopNotificationsTest>
    LoggingNotificationProxy;

class MockBalloonCollection : public BalloonCollectionImpl {
 public:
  MockBalloonCollection();
  virtual ~MockBalloonCollection();

  
  
  static const int kMockBalloonSpace;
  int max_balloon_count() const { return kMockBalloonSpace; }

  
  virtual void Add(const Notification& notification,
                   Profile* profile);
  virtual bool HasSpace() const;
  virtual Balloon* MakeBalloon(const Notification& notification,
                               Profile* profile);
  virtual void DisplayChanged() {}
  virtual void OnBalloonClosed(Balloon* source);
  virtual const BalloonCollection::Balloons& GetActiveBalloons();

  
  std::deque<Balloon*>& balloons() { return balloons_; }
  int count() const { return balloons_.size(); }

  
  int UppermostVerticalPosition();

  
  int MinHeight() { return Layout::min_balloon_height(); }
  int MaxHeight() { return Layout::max_balloon_height(); }

  
  gfx::Rect GetBalloonsBoundingBox() {
    return BalloonCollectionImpl::GetBalloonsBoundingBox();
  }

 private:
  std::deque<Balloon*> balloons_;
};

class DesktopNotificationsTest : public testing::Test {
 public:
  DesktopNotificationsTest();
  virtual ~DesktopNotificationsTest();

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

#endif  
