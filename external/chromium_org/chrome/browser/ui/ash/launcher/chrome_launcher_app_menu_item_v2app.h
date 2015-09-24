// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_CHROME_LAUNCHER_APP_MENU_ITEM_V2APP_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_CHROME_LAUNCHER_APP_MENU_ITEM_V2APP_H_

#include <string>

#include "chrome/browser/ui/ash/launcher/chrome_launcher_app_menu_item.h"

namespace gfx {
class image;
}

class ChromeLauncherController;

class ChromeLauncherAppMenuItemV2App : public ChromeLauncherAppMenuItem {
 public:
  ChromeLauncherAppMenuItemV2App(
      const base::string16 title,
      const gfx::Image* icon,
      const std::string& app_id,
      ChromeLauncherController* launcher_controller,
      int app_index,
      bool has_leading_separator);
  virtual bool IsEnabled() const OVERRIDE;
  virtual void Execute(int event_flags) OVERRIDE;

 private:
  
  ChromeLauncherController* launcher_controller_;

  
  const std::string app_id_;

  
  const int app_index_;

  DISALLOW_COPY_AND_ASSIGN(ChromeLauncherAppMenuItemV2App);
};

#endif  
