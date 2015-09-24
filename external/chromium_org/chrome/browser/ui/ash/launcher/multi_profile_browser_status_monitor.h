// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_MULTI_PROFILE_BROWSER_STATUS_MONITOR_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_MULTI_PROFILE_BROWSER_STATUS_MONITOR_H_

#include "chrome/browser/ui/ash/launcher/browser_status_monitor.h"

class MultiProfileBrowserStatusMonitor : public BrowserStatusMonitor {
 public:
  explicit MultiProfileBrowserStatusMonitor(
      ChromeLauncherController* launcher_controller);
  virtual ~MultiProfileBrowserStatusMonitor();

  
  virtual void ActiveUserChanged(const std::string& user_email) OVERRIDE;
  virtual void AddV1AppToShelf(Browser* browser) OVERRIDE;
  virtual void RemoveV1AppFromShelf(Browser* browser) OVERRIDE;

 private:
  typedef std::vector<Browser*> AppList;
  AppList app_list_;

  
  void ConnectV1AppToLauncher(Browser* browser);

  
  void DisconnectV1AppFromLauncher(Browser* browser);

  
  ChromeLauncherController* launcher_controller_;

  DISALLOW_COPY_AND_ASSIGN(MultiProfileBrowserStatusMonitor);
};

#endif  
