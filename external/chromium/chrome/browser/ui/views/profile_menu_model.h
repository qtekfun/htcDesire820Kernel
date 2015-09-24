// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PROFILE_MENU_MODEL_H_
#define CHROME_BROWSER_UI_VIEWS_PROFILE_MENU_MODEL_H_
#pragma once

#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/point.h"

namespace ui {
class Accelerator;
}

namespace views {
class Menu2;


class ProfileMenuModel : public ui::SimpleMenuModel,
                         public ui::SimpleMenuModel::Delegate {
 public:
  ProfileMenuModel();
  virtual ~ProfileMenuModel();

  void RunMenuAt(const gfx::Point& point);

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

 private:
  scoped_ptr<views::Menu2> menu_;

  DISALLOW_COPY_AND_ASSIGN(ProfileMenuModel);
};

}  

#endif  


