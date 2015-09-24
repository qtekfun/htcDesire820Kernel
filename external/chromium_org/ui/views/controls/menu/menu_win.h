// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_WIN_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_WIN_H_

#include <windows.h>

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/menu/menu.h"

namespace views {

namespace {
class MenuHostWindow;
}

class MenuWin : public Menu {
 public:
  
  
  
  
  
  
  
  
  MenuWin(Delegate* d, AnchorPoint anchor, HWND owner);
  
  
  
  
  explicit MenuWin(HMENU hmenu);
  virtual ~MenuWin();

  
  virtual void AddMenuItemWithIcon(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon) OVERRIDE;
  virtual Menu* AddSubMenuWithIcon(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon) OVERRIDE;
  virtual void AddSeparator(int index) OVERRIDE;
  virtual void EnableMenuItemByID(int item_id, bool enabled) OVERRIDE;
  virtual void EnableMenuItemAt(int index, bool enabled) OVERRIDE;
  virtual void SetMenuLabel(int item_id, const string16& label) OVERRIDE;
  virtual bool SetIcon(const gfx::ImageSkia& icon, int item_id) OVERRIDE;
  virtual void RunMenuAt(int x, int y) OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual int ItemCount() OVERRIDE;

  virtual HMENU GetMenuHandle() const {
    return menu_;
  }

  
  DWORD GetTPMAlignFlags() const;

 protected:
  virtual void AddMenuItemInternal(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon,
                                   MenuItemType type) OVERRIDE;

 private:
  friend class MenuHostWindow;

  
  struct ItemData;

  void AddMenuItemInternal(int index,
                           int item_id,
                           const string16& label,
                           const gfx::ImageSkia& icon,
                           HMENU submenu,
                           MenuItemType type);

  explicit MenuWin(MenuWin* parent);

  
  void SetMenuInfo();

  
  
  
  UINT GetStateFlagsForItemID(int item_id) const;

  
  HMENU menu_;

  
  
  HWND owner_;

  
  
  
  std::vector<string16> labels_;

  
  
  
  bool owner_draw_;

  
  
  
  
  
  
  std::vector<ItemData*> item_data_;

  
  std::vector<MenuWin*> submenus_;

  
  bool is_menu_visible_;

  DISALLOW_COPY_AND_ASSIGN(MenuWin);
};

}  

#endif  
