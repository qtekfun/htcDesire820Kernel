// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_WRAPPER_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_WRAPPER_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace gfx {
class Point;
}

namespace ui {
class MenuModel;
}

namespace views {

class MenuInsertionDelegateWin;
class MenuListener;

class VIEWS_EXPORT MenuWrapper {
 public:
  
  enum MenuAction {
    MENU_ACTION_NONE,      
    MENU_ACTION_SELECTED,  
    MENU_ACTION_PREVIOUS,  
    MENU_ACTION_NEXT,      
  };

  virtual ~MenuWrapper() {}

  
  static MenuWrapper* CreateWrapper(ui::MenuModel* model);

  
  virtual void RunMenuAt(const gfx::Point& point, int alignment) = 0;

  
  virtual void CancelMenu() = 0;

  
  
  virtual void Rebuild(MenuInsertionDelegateWin* delegate) = 0;

  
  
  virtual void UpdateStates() = 0;

  
  virtual HMENU GetNativeMenu() const = 0;

  
  
  
  virtual MenuAction GetMenuAction() const = 0;

  
  virtual void AddMenuListener(MenuListener* listener) = 0;

  
  virtual void RemoveMenuListener(MenuListener* listener) = 0;

  
  virtual void SetMinimumWidth(int width) = 0;
};

}  

#endif  
