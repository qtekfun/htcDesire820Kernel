// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_APP_SHORTCUT_LAUNCHER_ITEM_CONTROLLER_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_APP_SHORTCUT_LAUNCHER_ITEM_CONTROLLER_H_

#include <string>

#include "base/time/time.h"
#include "chrome/browser/ui/ash/launcher/launcher_item_controller.h"
#include "url/gurl.h"

class URLPattern;

namespace aura {
class Window;
}

namespace extensions {
class Extension;
}

class ChromeLauncherController;

class AppShortcutLauncherItemController : public LauncherItemController {
 public:
  AppShortcutLauncherItemController(const std::string& app_id,
                                    ChromeLauncherController* controller);

  virtual ~AppShortcutLauncherItemController();

  std::vector<content::WebContents*> GetRunningApplications();

  
  virtual bool IsOpen() const OVERRIDE;
  virtual bool IsVisible() const OVERRIDE;
  virtual void Launch(ash::LaunchSource source, int event_flags) OVERRIDE;
  virtual bool Activate(ash::LaunchSource source) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual ChromeLauncherAppMenuItems GetApplicationList(
      int event_flags) OVERRIDE;
  virtual bool ItemSelected(const ui::Event& event) OVERRIDE;
  virtual base::string16 GetTitle() OVERRIDE;
  virtual ui::MenuModel* CreateContextMenu(
      aura::Window* root_window) OVERRIDE;
  virtual ash::ShelfMenuModel* CreateApplicationMenu(int event_flags) OVERRIDE;
  virtual bool IsDraggable() OVERRIDE;
  virtual bool ShouldShowTooltip() OVERRIDE;

  
  
  const GURL& refocus_url() const { return refocus_url_; }
  
  void set_refocus_url(const GURL& refocus_url) { refocus_url_ = refocus_url; }

 private:
  
  content::WebContents* GetLRUApplication();

  
  
  
  
  
  bool WebContentMatchesApp(const extensions::Extension* extension,
                            const URLPattern& refocus_pattern,
                            content::WebContents* web_contents,
                            bool is_app);

  
  void ActivateContent(content::WebContents* content);

  
  
  bool AdvanceToNextApp();

  
  bool IsV2App();

  
  bool AllowNextLaunchAttempt();

  GURL refocus_url_;

  
  
  base::Time last_launch_attempt_;

  ChromeLauncherController* chrome_launcher_controller_;

  DISALLOW_COPY_AND_ASSIGN(AppShortcutLauncherItemController);
};

#endif  
