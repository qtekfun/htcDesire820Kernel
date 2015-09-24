// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_TEST_UTIL_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_TEST_UTIL_H_
#pragma once

#include <string>

#include "chrome/browser/notifications/notification_object_proxy.h"
#include "chrome/browser/notifications/balloon.h"
#include "ui/gfx/size.h"

class MockNotificationDelegate : public NotificationDelegate {
 public:
  explicit MockNotificationDelegate(const std::string& id);
  virtual ~MockNotificationDelegate();

  
  virtual void Display() {}
  virtual void Error() {}
  virtual void Close(bool by_user) {}
  virtual void Click() {}
  virtual std::string id() const;

 private:
  std::string id_;

  DISALLOW_COPY_AND_ASSIGN(MockNotificationDelegate);
};

template<class Logger>
class LoggingNotificationDelegate : public NotificationDelegate {
 public:
  explicit LoggingNotificationDelegate(std::string id)
      : notification_id_(id) {
  }

  
  virtual void Display() {
    Logger::log("notification displayed\n");
  }
  virtual void Error() {
    Logger::log("notification error\n");
  }
  virtual void Click() {
    Logger::log("notification clicked\n");
  }
  virtual void Close(bool by_user) {
    if (by_user)
      Logger::log("notification closed by user\n");
    else
      Logger::log("notification closed by script\n");
  }
  virtual std::string id() const {
    return notification_id_;
  }
 private:
  std::string notification_id_;

  DISALLOW_COPY_AND_ASSIGN(LoggingNotificationDelegate);
};

class MockBalloonView : public BalloonView {
 public:
  explicit MockBalloonView(Balloon * balloon) :
      balloon_(balloon) {}

  
  virtual void Show(Balloon* balloon) {}
  virtual void Update() {}
  virtual void RepositionToBalloon() {}
  virtual void Close(bool by_user);
  virtual gfx::Size GetSize() const;
  virtual BalloonHost* GetHost() const;

 private:
  
  Balloon* balloon_;
};

#endif  
