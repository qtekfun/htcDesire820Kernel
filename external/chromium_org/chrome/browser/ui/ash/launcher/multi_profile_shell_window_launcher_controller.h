// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_MULTI_PROFILE_SHELL_WINDOW_LAUNCHER_CONTROLLER_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_MULTI_PROFILE_SHELL_WINDOW_LAUNCHER_CONTROLLER_H_

#include "chrome/browser/ui/ash/launcher/shell_window_launcher_controller.h"

class MultiProfileShellWindowLauncherController
    : public ShellWindowLauncherController {
 public:
  explicit MultiProfileShellWindowLauncherController(
      ChromeLauncherController* owner);
  virtual ~MultiProfileShellWindowLauncherController();

  
  virtual void ActiveUserChanged(const std::string& user_email) OVERRIDE;
  virtual void AdditionalUserAddedToSession(Profile* profile) OVERRIDE;

  
  virtual void OnShellWindowAdded(apps::ShellWindow* shell_window) OVERRIDE;
  virtual void OnShellWindowRemoved(apps::ShellWindow* shell_window) OVERRIDE;

 private:
  typedef std::vector<apps::ShellWindow*> ShellWindowList;
  typedef std::vector<apps::ShellWindowRegistry*> ShellWindowRegistryList;

  
  ShellWindowList shell_window_list_;

  
  ShellWindowRegistryList multi_user_registry_;

  DISALLOW_COPY_AND_ASSIGN(MultiProfileShellWindowLauncherController);
};

#endif  
