// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_ITEM_DELEGATE_H_
#define ASH_SHELF_SHELF_ITEM_DELEGATE_H_

#include "ash/ash_export.h"
#include "base/strings/string16.h"

namespace aura {
class Window;
}

namespace ui {
class Event;
class MenuModel;
}

namespace ash {

class ShelfMenuModel;

class ASH_EXPORT ShelfItemDelegate {
 public:
  virtual ~ShelfItemDelegate() {}

  
  
  
  
  
  
  
  virtual bool ItemSelected(const ui::Event& event) = 0;

  
  virtual base::string16 GetTitle() = 0;

  
  
  
  virtual ui::MenuModel* CreateContextMenu(aura::Window* root_window) = 0;

  
  
  
  
  
  
  
  
  
  virtual ShelfMenuModel* CreateApplicationMenu(int event_flags) = 0;

  
  virtual bool IsDraggable() = 0;

  
  virtual bool ShouldShowTooltip() = 0;
};

}  

#endif  
