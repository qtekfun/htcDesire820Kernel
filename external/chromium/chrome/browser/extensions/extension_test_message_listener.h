// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TEST_MESSAGE_LISTENER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TEST_MESSAGE_LISTENER_H_
#pragma once

#include <string>

#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ExtensionTestSendMessageFunction;

class ExtensionTestMessageListener : public NotificationObserver {
 public:
  
  ExtensionTestMessageListener(const std::string& expected_message,
                               bool will_reply);
  ~ExtensionTestMessageListener();

  
  
  
  bool WaitUntilSatisfied();

  
  
  void Reply(const std::string& message);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  bool was_satisfied() const { return satisfied_; }

 private:
  NotificationRegistrar registrar_;

  
  std::string expected_message_;

  
  bool satisfied_;

  
  
  bool waiting_;

  
  
  bool will_reply_;

  
  ExtensionTestSendMessageFunction* function_;
};

#endif  
