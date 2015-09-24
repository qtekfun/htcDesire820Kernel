// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_LAUNCHER_CONTEXT_MENU_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_LAUNCHER_CONTEXT_MENU_H_

#include "ash/launcher/launcher_types.h"
#include "ash/shelf/shelf_alignment_menu.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/models/simple_menu_model.h"

class ChromeLauncherController;

namespace aura {
class Window;
}

namespace extensions {
class ContextMenuMatcher;
}

class LauncherContextMenu : public ui::SimpleMenuModel,
                            public ui::SimpleMenuModel::Delegate {
 public:
  
  
  LauncherContextMenu(ChromeLauncherController* controller,
                      const ash::LauncherItem* item,
                      aura::Window* root_window);
  
  LauncherContextMenu(ChromeLauncherController* controller,
                      aura::Window* root_window);
  virtual ~LauncherContextMenu();

  void Init();

  
  ash::LauncherID id() const { return item_.id; }

  
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual base::string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(
      LauncherContextMenuTest,
      NewIncognitoWindowMenuIsDisabledWhenIncognitoModeOff);
  FRIEND_TEST_ALL_PREFIXES(
      LauncherContextMenuTest,
      NewWindowMenuIsDisabledWhenIncognitoModeForced);

  enum MenuItem {
    MENU_OPEN_NEW,
    MENU_CLOSE,
    MENU_PIN,
    LAUNCH_TYPE_PINNED_TAB,
    LAUNCH_TYPE_REGULAR_TAB,
    LAUNCH_TYPE_FULLSCREEN,
    LAUNCH_TYPE_WINDOW,
    MENU_AUTO_HIDE,
    MENU_NEW_WINDOW,
    MENU_NEW_INCOGNITO_WINDOW,
    MENU_ALIGNMENT_MENU,
#if defined(OS_CHROMEOS)
    MENU_CHANGE_WALLPAPER,
#endif
  };

  
  
  bool is_valid_item() const { return item_.id != 0; }

  ChromeLauncherController* controller_;

  ash::LauncherItem item_;

  ash::ShelfAlignmentMenu shelf_alignment_menu_;

  scoped_ptr<extensions::ContextMenuMatcher> extension_items_;

  aura::Window* root_window_;

  DISALLOW_COPY_AND_ASSIGN(LauncherContextMenu);
};

#endif  
