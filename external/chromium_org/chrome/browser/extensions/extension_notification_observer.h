// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_NOTIFICATION_OBSERVER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_NOTIFICATION_OBSERVER_H_

#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/chrome_notification_types.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_source.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace extensions {

class ExtensionNotificationObserver : public content::NotificationObserver {
 public:
  ExtensionNotificationObserver(content::NotificationSource source,
                                const std::set<std::string>& extension_ids);

  virtual ~ExtensionNotificationObserver();

  
  
  
  testing::AssertionResult CheckNotifications() WARN_UNUSED_RESULT;
  testing::AssertionResult CheckNotifications(
      chrome::NotificationType type) WARN_UNUSED_RESULT;
  testing::AssertionResult CheckNotifications(
      chrome::NotificationType t1,
      chrome::NotificationType t2) WARN_UNUSED_RESULT;
  testing::AssertionResult CheckNotifications(
      chrome::NotificationType t1,
      chrome::NotificationType t2,
      chrome::NotificationType t3) WARN_UNUSED_RESULT;
  testing::AssertionResult CheckNotifications(
      chrome::NotificationType t1,
      chrome::NotificationType t2,
      chrome::NotificationType t3,
      chrome::NotificationType t4,
      chrome::NotificationType t5,
      chrome::NotificationType t6) WARN_UNUSED_RESULT;

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  testing::AssertionResult CheckNotifications(
      const std::vector<chrome::NotificationType>& types);

  const std::set<std::string> extension_ids_;
  std::vector<chrome::NotificationType> notifications_;
  content::NotificationRegistrar registrar_;
};

}  

#endif  
