// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_NATIVE_MENU_WIN_H_
#define UI_VIEWS_CONTROLS_MENU_NATIVE_MENU_WIN_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "ui/views/controls/menu/menu_wrapper.h"
#include "ui/views/views_export.h"

namespace ui {
class MenuModel;
}

namespace views {

class VIEWS_EXPORT NativeMenuWin : public MenuWrapper {
 public:
  
  
  
  NativeMenuWin(ui::MenuModel* model, HWND system_menu_for);
  virtual ~NativeMenuWin();

  
  virtual void RunMenuAt(const gfx::Point& point, int alignment) OVERRIDE;
  virtual void CancelMenu() OVERRIDE;
  virtual void Rebuild(MenuInsertionDelegateWin* delegate) OVERRIDE;
  virtual void UpdateStates() OVERRIDE;
  virtual HMENU GetNativeMenu() const OVERRIDE;
  virtual MenuAction GetMenuAction() const OVERRIDE;
  virtual void AddMenuListener(MenuListener* listener) OVERRIDE;
  virtual void RemoveMenuListener(MenuListener* listener) OVERRIDE;
  virtual void SetMinimumWidth(int width) OVERRIDE;

 private:
  
  
  
  
  
  
  
  
  
  
  

  struct HighlightedMenuItemInfo;

  
  bool IsSeparatorItemAt(int menu_index) const;

  
  void AddMenuItemAt(int menu_index, int model_index);
  void AddSeparatorItemAt(int menu_index, int model_index);

  
  void SetMenuItemState(int menu_index,
                        bool enabled,
                        bool checked,
                        bool is_default);

  
  void SetMenuItemLabel(int menu_index,
                        int model_index,
                        const string16& label);

  
  
  
  
  void UpdateMenuItemInfoForString(MENUITEMINFO* mii,
                                   int model_index,
                                   const string16& label);

  
  
  UINT GetAlignmentFlags(int alignment) const;

  
  
  void ResetNativeMenu();

  
  void CreateHostWindow();

  
  void DelayedSelect();

  
  
  static bool GetHighlightedMenuItemInfo(HMENU menu,
                                         HighlightedMenuItemInfo* info);

  
  static LRESULT CALLBACK MenuMessageHook(
      int n_code, WPARAM w_param, LPARAM l_param);

  
  ui::MenuModel* model_;

  HMENU menu_;

  
  
  bool owner_draw_;

  
  
  struct ItemData;
  std::vector<ItemData*> items_;

  
  class MenuHostWindow;
  friend MenuHostWindow;
  scoped_ptr<MenuHostWindow> host_window_;

  
  
  HWND system_menu_for_;

  
  int first_item_index_;

  
  MenuAction menu_action_;

  
  ObserverList<MenuListener> listeners_;

  
  
  bool listeners_called_;

  
  NativeMenuWin* menu_to_select_;
  int position_to_select_;
  base::WeakPtrFactory<NativeMenuWin> menu_to_select_factory_;

  
  NativeMenuWin* parent_;

  
  
  
  bool* destroyed_flag_;

  
  
  
  static NativeMenuWin* open_native_menu_win_;

  DISALLOW_COPY_AND_ASSIGN(NativeMenuWin);
};

}  

#endif  
