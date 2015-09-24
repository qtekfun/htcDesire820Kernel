// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_JAVASCRIPT_TEST_OBSERVER_H_
#define CHROME_TEST_BASE_JAVASCRIPT_TEST_OBSERVER_H_

#include <string>

#include "base/compiler_specific.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace base {
class DictionaryValue;
}

namespace content {
class RenderViewHost;
}

class TestMessageHandler {
 public:
  enum MessageResponse {
    
    CONTINUE,
    
    DONE
  };

  TestMessageHandler();
  virtual ~TestMessageHandler();

  
  virtual MessageResponse HandleMessage(const std::string& json) = 0;

  void SetError(const std::string& message);

  bool ok() const {
    return ok_;
  }

  const std::string& error_message() const {
    return error_message_;
  }

  
  virtual void Reset();

 private:
  bool ok_;
  std::string error_message_;
};

class JavascriptTestObserver : public content::NotificationObserver {
 public:
  
  
  JavascriptTestObserver(
      content::RenderViewHost* render_view_host,
      TestMessageHandler* handler);

  virtual ~JavascriptTestObserver();

  
  
  
  bool Run();

  
  
  void Reset();

  virtual void Observe(
      int type,
      const content::NotificationSource& source,
      const content::NotificationDetails& details) OVERRIDE;

 private:
  
  void Continue();

  
  void EndTest();

  TestMessageHandler* handler_;
  bool running_;
  bool finished_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(JavascriptTestObserver);
};

#endif  
