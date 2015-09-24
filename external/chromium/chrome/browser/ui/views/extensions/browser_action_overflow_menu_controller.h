// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_BROWSER_ACTION_OVERFLOW_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_BROWSER_ACTION_OVERFLOW_MENU_CONTROLLER_H_
#pragma once

#include <set>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "views/controls/menu/menu_delegate.h"

class BrowserActionsContainer;
class BrowserActionView;
class ExtensionContextMenuModel;

namespace views {
class Menu2;
}

class BrowserActionOverflowMenuController : public views::MenuDelegate {
 public:
  
  class Observer {
   public:
    virtual void NotifyMenuDeleted(
        BrowserActionOverflowMenuController* controller) = 0;
  };

  BrowserActionOverflowMenuController(
      BrowserActionsContainer* owner,
      views::MenuButton* menu_button,
      const std::vector<BrowserActionView*>& views,
      int start_index);

  void set_observer(Observer* observer) { observer_ = observer; }

  
  bool RunMenu(gfx::NativeWindow window, bool for_drop);

  
  void CancelMenu();

  
  virtual void ExecuteCommand(int id);
  virtual bool ShowContextMenu(views::MenuItemView* source,
                               int id,
                               const gfx::Point& p,
                               bool is_mouse_gesture);
  virtual void DropMenuClosed(views::MenuItemView* menu);
  
  
  virtual bool GetDropFormats(
      views::MenuItemView* menu,
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats);
  virtual bool AreDropTypesRequired(views::MenuItemView* menu);
  virtual bool CanDrop(views::MenuItemView* menu, const ui::OSExchangeData& data);
  virtual int GetDropOperation(views::MenuItemView* item,
                               const views::DropTargetEvent& event,
                               DropPosition* position);
  virtual int OnPerformDrop(views::MenuItemView* menu,
                            DropPosition position,
                            const views::DropTargetEvent& event);
  
  
  virtual bool CanDrag(views::MenuItemView* menu);
  virtual void WriteDragData(views::MenuItemView* sender, ui::OSExchangeData* data);
  virtual int GetDragOperations(views::MenuItemView* sender);

 private:
  
  virtual ~BrowserActionOverflowMenuController();

  
  
  
  BrowserActionView* ViewForId(int id, size_t* index);

  
  BrowserActionsContainer* owner_;

  
  Observer* observer_;

  
  views::MenuButton* menu_button_;

  
  scoped_ptr<views::MenuItemView> menu_;

  
  
  const std::vector<BrowserActionView*>* views_;

  
  
  int start_index_;

  
  bool for_drop_;

  
  scoped_refptr<ExtensionContextMenuModel> context_menu_contents_;
  scoped_ptr<views::Menu2> context_menu_menu_;

  friend class DeleteTask<BrowserActionOverflowMenuController>;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionOverflowMenuController);
};

#endif  
