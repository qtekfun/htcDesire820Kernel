// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_MODEL_ADAPTER_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_MODEL_ADAPTER_H_

#include <map>

#include "ui/views/controls/menu/menu_delegate.h"

namespace ui {
class MenuModel;
}

namespace views {
class MenuItemView;

class VIEWS_EXPORT MenuModelAdapter : public MenuDelegate {
 public:
  
  
  explicit MenuModelAdapter(ui::MenuModel* menu_model);
  virtual ~MenuModelAdapter();

  
  
  virtual void BuildMenu(MenuItemView* menu);

  
  
  MenuItemView* CreateMenu();

  void set_triggerable_event_flags(int triggerable_event_flags) {
    triggerable_event_flags_ = triggerable_event_flags;
  }
  int triggerable_event_flags() const { return triggerable_event_flags_; }

  
  
  static MenuItemView* AddMenuItemFromModelAt(ui::MenuModel* model,
                                              int model_index,
                                              MenuItemView* menu,
                                              int menu_index,
                                              int item_id);

  
  
  static MenuItemView* AppendMenuItemFromModel(ui::MenuModel* model,
                                               int model_index,
                                               MenuItemView* menu,
                                               int item_id);

 protected:
  
  
  
  virtual MenuItemView* AppendMenuItem(MenuItemView* menu,
                                       ui::MenuModel* model,
                                       int index);

  
  virtual void ExecuteCommand(int id) OVERRIDE;
  virtual void ExecuteCommand(int id, int mouse_event_flags) OVERRIDE;
  virtual bool IsTriggerableEvent(MenuItemView* source,
                                  const ui::Event& e) OVERRIDE;
  virtual bool GetAccelerator(int id,
                              ui::Accelerator* accelerator) OVERRIDE;
  virtual string16 GetLabel(int id) const OVERRIDE;
  virtual const gfx::Font* GetLabelFont(int id) const OVERRIDE;
  virtual bool IsCommandEnabled(int id) const OVERRIDE;
  virtual bool IsItemChecked(int id) const OVERRIDE;
  virtual void SelectionChanged(MenuItemView* menu) OVERRIDE;
  virtual void WillShowMenu(MenuItemView* menu) OVERRIDE;
  virtual void WillHideMenu(MenuItemView* menu) OVERRIDE;

 private:
  
  void BuildMenuImpl(MenuItemView* menu, ui::MenuModel* model);

  
  
  
  ui::MenuModel* menu_model_;

  
  int triggerable_event_flags_;

  
  std::map<MenuItemView*, ui::MenuModel*> menu_map_;

  DISALLOW_COPY_AND_ASSIGN(MenuModelAdapter);
};

}  

#endif  
