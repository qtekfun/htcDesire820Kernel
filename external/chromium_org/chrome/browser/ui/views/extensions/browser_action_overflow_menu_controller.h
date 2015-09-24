// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_BROWSER_ACTION_OVERFLOW_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_BROWSER_ACTION_OVERFLOW_MENU_CONTROLLER_H_

#include <set>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "ui/views/controls/menu/menu_delegate.h"

class Browser;
class BrowserActionsContainer;
class BrowserActionView;

namespace views {
class MenuRunner;
class Widget;
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
      Browser* browser,
      views::MenuButton* menu_button,
      const std::vector<BrowserActionView*>& views,
      int start_index);

  void set_observer(Observer* observer) { observer_ = observer; }

  
  bool RunMenu(views::Widget* widget, bool for_drop);

  
  void CancelMenu();

  
  virtual bool IsCommandEnabled(int id) const OVERRIDE;
  virtual void ExecuteCommand(int id) OVERRIDE;
  virtual bool ShowContextMenu(views::MenuItemView* source,
                               int id,
                               const gfx::Point& p,
                               ui::MenuSourceType source_type) OVERRIDE;
  virtual void DropMenuClosed(views::MenuItemView* menu) OVERRIDE;
  
  
  virtual bool GetDropFormats(
      views::MenuItemView* menu,
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired(views::MenuItemView* menu) OVERRIDE;
  virtual bool CanDrop(views::MenuItemView* menu,
                       const ui::OSExchangeData& data) OVERRIDE;
  virtual int GetDropOperation(views::MenuItemView* item,
                               const ui::DropTargetEvent& event,
                               DropPosition* position) OVERRIDE;
  virtual int OnPerformDrop(views::MenuItemView* menu,
                            DropPosition position,
                            const ui::DropTargetEvent& event) OVERRIDE;
  
  
  virtual bool CanDrag(views::MenuItemView* menu) OVERRIDE;
  virtual void WriteDragData(views::MenuItemView* sender,
                             ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperations(views::MenuItemView* sender) OVERRIDE;

 private:
  
  virtual ~BrowserActionOverflowMenuController();

  
  
  
  BrowserActionView* ViewForId(int id, size_t* index);

  
  BrowserActionsContainer* owner_;

  Browser* browser_;

  
  Observer* observer_;

  
  views::MenuButton* menu_button_;

  
  views::MenuItemView* menu_;

  
  scoped_ptr<views::MenuRunner> menu_runner_;

  
  
  const std::vector<BrowserActionView*>* views_;

  
  
  int start_index_;

  
  bool for_drop_;

  friend class base::DeleteHelper<BrowserActionOverflowMenuController>;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionOverflowMenuController);
};

#endif  
