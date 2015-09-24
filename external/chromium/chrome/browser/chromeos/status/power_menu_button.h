// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_POWER_MENU_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_POWER_MENU_BUTTON_H_
#pragma once

#include "chrome/browser/chromeos/cros/power_library.h"
#include "chrome/browser/chromeos/status/status_area_button.h"
#include "ui/base/models/menu_model.h"
#include "views/controls/menu/menu_2.h"
#include "views/controls/menu/view_menu_delegate.h"

namespace base {
class TimeDelta;
}

class SkBitmap;

namespace chromeos {

class PowerMenuButton : public StatusAreaButton,
                        public views::ViewMenuDelegate,
                        public ui::MenuModel,
                        public PowerLibrary::Observer {
 public:
  PowerMenuButton(StatusAreaHost* host);
  virtual ~PowerMenuButton();

  
  virtual bool HasIcons() const  { return false; }
  virtual int GetItemCount() const;
  virtual ui::MenuModel::ItemType GetTypeAt(int index) const;
  virtual int GetCommandIdAt(int index) const { return index; }
  virtual string16 GetLabelAt(int index) const;
  virtual bool IsItemDynamicAt(int index) const { return true; }
  virtual bool GetAcceleratorAt(int index,
      ui::Accelerator* accelerator) const { return false; }
  virtual bool IsItemCheckedAt(int index) const { return false; }
  virtual int GetGroupIdAt(int index) const { return 0; }
  virtual bool GetIconAt(int index, SkBitmap* icon) { return false; }
  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(int index) const {
    return NULL;
  }
  virtual bool IsEnabledAt(int index) const { return false; }
  virtual ui::MenuModel* GetSubmenuModelAt(int index) const { return NULL; }
  virtual void HighlightChangedTo(int index) {}
  virtual void ActivatedAt(int index) {}
  virtual void MenuWillShow() {}
  virtual void SetMenuModelDelegate(ui::MenuModelDelegate* delegate) {}

  
  virtual void PowerChanged(PowerLibrary* obj);
  virtual void SystemResumed() {}

  int icon_id() const { return icon_id_; }

 protected:
  virtual int icon_width() { return 26; }

 private:
  
  virtual void OnLocaleChanged() OVERRIDE;

  
  virtual void RunMenu(views::View* source, const gfx::Point& pt);

  
  void UpdateIconAndLabelInfo();

  
  static const int kNumPowerImages;

  
  bool battery_is_present_;
  bool line_power_on_;
  bool battery_fully_charged_;
  double battery_percentage_;
  base::TimeDelta battery_time_to_full_;
  base::TimeDelta battery_time_to_empty_;

  
  int icon_id_;

  
  
  views::Menu2 power_menu_;

  DISALLOW_COPY_AND_ASSIGN(PowerMenuButton);
};

}  

#endif  
