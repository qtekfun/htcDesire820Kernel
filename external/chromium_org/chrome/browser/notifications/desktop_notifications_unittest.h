// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATIONS_UNITTEST_H_
#define CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATIONS_UNITTEST_H_

#include <deque>
#include <string>

#include "base/message_loop/message_loop.h"
#include "base/prefs/testing_pref_service.h"
#include "chrome/browser/notifications/balloon_collection_impl.h"
#include "chrome/browser/notifications/balloon_notification_ui_manager.h"
#include "chrome/browser/notifications/desktop_notification_service.h"
#include "chrome/browser/notifications/notification.h"
#include "chrome/browser/notifications/notification_test_util.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "content/public/test/render_view_test.h"
#include "content/public/test/test_browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

class ActiveDesktopMonitor;
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
                   Profile* profile) OVERRIDE;
  virtual bool HasSpace() const OVERRIDE;
  virtual Balloon* MakeBalloon(const Notification& notification,
                               Profile* profile) OVERRIDE;
  virtual void DisplayChanged() OVERRIDE {}
  virtual void OnBalloonClosed(Balloon* source) OVERRIDE;
  virtual const BalloonCollection::Balloons& GetActiveBalloons() OVERRIDE;

  
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
  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  void AllowOrigin(const GURL& origin) {
    service_->GrantPermission(origin);
  }

  void DenyOrigin(const GURL& origin) {
    service_->DenyPermission(origin);
  }

  
  content::ShowDesktopNotificationHostMsgParams StandardTestNotification();

  
  
  TestingBrowserProcessInitializer initializer_;

  
  
  base::MessageLoopForUI message_loop_;
  content::TestBrowserThread ui_thread_;

  
  TestingPrefServiceSimple local_state_;

  
  scoped_ptr<TestingProfile> profile_;

  
  MockBalloonCollection* balloon_collection_;

  
  scoped_ptr<BalloonNotificationUIManager> ui_manager_;

  
  scoped_ptr<DesktopNotificationService> service_;

  
  static std::string log_output_;
};

#endif  
