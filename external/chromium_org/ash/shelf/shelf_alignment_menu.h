// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_ALIGNMENT_MENU_H_
#define ASH_SHELF_SHELF_ALIGNMENT_MENU_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "ui/base/models/simple_menu_model.h"

namespace aura {
class RootWindow;
}

namespace ash {

class ASH_EXPORT ShelfAlignmentMenu : public ui::SimpleMenuModel,
                                      public ui::SimpleMenuModel::Delegate {
 public:
  explicit ShelfAlignmentMenu(aura::Window* root);
  virtual ~ShelfAlignmentMenu();

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  enum MenuItem {
    
    MENU_ALIGN_LEFT = 500,
    MENU_ALIGN_RIGHT,
    MENU_ALIGN_BOTTOM,
  };

  aura::Window* root_window_;

  DISALLOW_COPY_AND_ASSIGN(ShelfAlignmentMenu);
};

}  

#endif  
