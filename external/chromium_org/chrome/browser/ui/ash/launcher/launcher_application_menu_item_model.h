// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_LAUNCHER_APPLICATION_MENU_ITEM_MODEL_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_LAUNCHER_APPLICATION_MENU_ITEM_MODEL_H_

#include "ash/shelf/shelf_menu_model.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

class ChromeLauncherAppMenuItem;

typedef ScopedVector<ChromeLauncherAppMenuItem> ChromeLauncherAppMenuItems;

class LauncherApplicationMenuItemModel : public ash::ShelfMenuModel,
                                         public ui::SimpleMenuModel::Delegate {
 public:
  explicit LauncherApplicationMenuItemModel(
      ChromeLauncherAppMenuItems item_list);
  virtual ~LauncherApplicationMenuItemModel();

  
  virtual bool IsCommandActive(int command_id) const OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  void Build();

  
  ChromeLauncherAppMenuItems launcher_items_;

  DISALLOW_COPY_AND_ASSIGN(LauncherApplicationMenuItemModel);
};

#endif  
