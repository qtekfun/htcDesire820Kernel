// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TEST_NOTIFICATION_OBSERVER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TEST_NOTIFICATION_OBSERVER_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_types.h"

namespace content {
class WindowedNotificationObserver;
}

class ExtensionTestNotificationObserver : public content::NotificationObserver {
 public:
  explicit ExtensionTestNotificationObserver(Browser* browser);
  virtual ~ExtensionTestNotificationObserver();

  
  bool WaitForPageActionCountChangeTo(int count);

  
  bool WaitForPageActionVisibilityChangeTo(int count);

  
  
  bool WaitForExtensionInstall();

  
  
  bool WaitForExtensionInstallError();

  
  void WaitForExtensionAndViewLoad();

  
  void WaitForExtensionLoad();

  
  
  bool WaitForExtensionLoadError();

  
  
  bool WaitForExtensionCrash(const std::string& extension_id);

  
  bool WaitForCrxInstallerDone();

  
  bool WaitForExtensionViewsToLoad();

  
  
  
  void Watch(int type, const content::NotificationSource& source);

  
  
  void Wait();

  const std::string& last_loaded_extension_id() {
    return last_loaded_extension_id_;
  }
  void set_last_loaded_extension_id(std::string last_loaded_extension_id) {
    last_loaded_extension_id_ = last_loaded_extension_id;
  }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  Profile* GetProfile();

  void WaitForNotification(int notification_type);

  Browser* browser_;
  Profile* profile_;

  content::NotificationRegistrar registrar_;
  scoped_ptr<content::WindowedNotificationObserver> observer_;

  std::string last_loaded_extension_id_;
  int extension_installs_observed_;
  int extension_load_errors_observed_;
  int crx_installers_done_observed_;
};

#endif  
