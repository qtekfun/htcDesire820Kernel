// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_NAVIGATION_OBSERVER_H_
#define CHROME_BROWSER_EXTENSIONS_NAVIGATION_OBSERVER_H_

#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace content {
class NavigationController;
}

namespace extensions {

class NavigationObserver : public ExtensionInstallPrompt::Delegate,
                           public content::NotificationObserver {
 public:
  explicit NavigationObserver(Profile* profile);
  virtual ~NavigationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
 private:
  
  void RegisterForNotifications();

  
  
  
  void PromptToEnableExtensionIfNecessary(
      content::NavigationController* nav_controller);

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  
  scoped_ptr<ExtensionInstallPrompt> extension_install_prompt_;

  
  std::string in_progress_prompt_extension_id_;
  content::NavigationController* in_progress_prompt_navigation_controller_;

  
  std::set<std::string> prompted_extensions_;

  DISALLOW_COPY_AND_ASSIGN(NavigationObserver);
};

}  

#endif  
