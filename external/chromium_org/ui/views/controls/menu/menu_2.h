// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_2_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_2_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/controls/menu/menu_wrapper.h"
#include "ui/views/views_export.h"

namespace gfx {
class Point;
}

namespace ui {
class MenuModel;
}

namespace views {

class VIEWS_EXPORT Menu2 {
 public:
  
  
  enum Alignment {
    ALIGN_TOPLEFT,
    ALIGN_TOPRIGHT
  };

  
  
  
  
  
  
  explicit Menu2(ui::MenuModel* model);
  virtual ~Menu2();

  
  
  
  void RunMenuAt(const gfx::Point& point, Alignment alignment);
  void RunContextMenuAt(const gfx::Point& point);

  
  void CancelMenu();

  
  
  void Rebuild();

  
  
  void UpdateStates();

  
  HMENU GetNativeMenu() const;

  
  
  
  MenuWrapper::MenuAction GetMenuAction() const;

  
  void AddMenuListener(MenuListener* listener);

  
  void RemoveMenuListener(MenuListener* listener);

  
  ui::MenuModel* model() const { return model_; }

  
  void SetMinimumWidth(int width);

 private:

  ui::MenuModel* model_;

  
  scoped_ptr<MenuWrapper> wrapper_;

  DISALLOW_COPY_AND_ASSIGN(Menu2);
};

}  

#endif  
