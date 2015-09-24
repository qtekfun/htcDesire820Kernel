// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TEST_MESSAGE_LISTENER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TEST_MESSAGE_LISTENER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace extensions {
class TestSendMessageFunction;
}

class ExtensionTestMessageListener : public content::NotificationObserver {
 public:
  
  ExtensionTestMessageListener(const std::string& expected_message,
                               bool will_reply);
  virtual ~ExtensionTestMessageListener();

  void AlsoListenForFailureMessage(const std::string& failure_message) {
    failure_message_ = failure_message;
  }

  
  
  
  bool WaitUntilSatisfied();

  
  
  void Reply(const std::string& message);

  
  void Reply(int message);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  bool was_satisfied() const { return satisfied_; }

 private:
  content::NotificationRegistrar registrar_;

  
  std::string expected_message_;

  
  bool satisfied_;

  
  
  bool waiting_;

  
  
  bool will_reply_;

  
  std::string failure_message_;

  
  bool failed_;

  
  scoped_refptr<extensions::TestSendMessageFunction> function_;
};

#endif  
