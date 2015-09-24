// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_BROWSER_SHORTCUT_LAUNCHER_ITEM_CONTROLLER_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_BROWSER_SHORTCUT_LAUNCHER_ITEM_CONTROLLER_H_

#include "chrome/browser/ui/ash/launcher/launcher_item_controller.h"

namespace content {
class WebContents;
}

namespace gfx {
class Image;
}

class Browser;
class ChromeLauncherController;

class BrowserShortcutLauncherItemController : public LauncherItemController {
 public:
  explicit BrowserShortcutLauncherItemController(
      ChromeLauncherController* controller);

  virtual ~BrowserShortcutLauncherItemController();

  
  void UpdateBrowserItemState();

  
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

 private:
  
  
  gfx::Image GetBrowserListIcon(content::WebContents* web_contents) const;

  
  
  base::string16 GetBrowserListTitle(content::WebContents* web_contents) const;

  
  bool IsIncognito(content::WebContents* web_contents) const;

  
  void ActivateOrAdvanceToNextBrowser();

  
  
  bool IsBrowserRepresentedInBrowserList(Browser* browser);

  DISALLOW_COPY_AND_ASSIGN(BrowserShortcutLauncherItemController);
};

#endif  
