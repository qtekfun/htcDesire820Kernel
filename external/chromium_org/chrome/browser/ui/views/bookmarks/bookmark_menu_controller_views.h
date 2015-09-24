// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_MENU_CONTROLLER_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_MENU_CONTROLLER_VIEWS_H_

#include <set>

#include "base/compiler_specific.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "ui/views/controls/menu/menu_delegate.h"
#include "ui/views/controls/menu/menu_item_view.h"

class BookmarkBarView;
class BookmarkMenuControllerObserver;
class BookmarkMenuDelegate;
class BookmarkNode;
class Browser;

namespace content {
class PageNavigator;
}

namespace ui {
class OSExchangeData;
}

namespace views {
class MenuButton;
class MenuRunner;
class Widget;
}

class BookmarkMenuController : public BaseBookmarkModelObserver,
                               public views::MenuDelegate {
 public:
  
  
  BookmarkMenuController(Browser* browser,
                         content::PageNavigator* page_navigator,
                         views::Widget* parent,
                         const BookmarkNode* node,
                         int start_child_index);

  void RunMenuAt(BookmarkBarView* bookmark_bar, bool for_drop);

  void clear_bookmark_bar() {
    bookmark_bar_ = NULL;
  }

  
  void Cancel();

  
  const BookmarkNode* node() const { return node_; }

  
  views::MenuItemView* menu() const;

  
  views::MenuItemView* context_menu() const;

  
  void SetPageNavigator(content::PageNavigator* navigator);

  void set_observer(BookmarkMenuControllerObserver* observer) {
    observer_ = observer;
  }

  
  virtual base::string16 GetTooltipText(int id,
                                        const gfx::Point& p) const OVERRIDE;
  virtual bool IsTriggerableEvent(views::MenuItemView* view,
                                  const ui::Event& e) OVERRIDE;
  virtual void ExecuteCommand(int id, int mouse_event_flags) OVERRIDE;
  virtual bool ShouldExecuteCommandWithoutClosingMenu(
      int id,
      const ui::Event& e) OVERRIDE;
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
  virtual bool ShowContextMenu(views::MenuItemView* source,
                               int id,
                               const gfx::Point& p,
                               ui::MenuSourceType source_type) OVERRIDE;
  virtual void DropMenuClosed(views::MenuItemView* menu) OVERRIDE;
  virtual bool CanDrag(views::MenuItemView* menu) OVERRIDE;
  virtual void WriteDragData(views::MenuItemView* sender,
                             ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperations(views::MenuItemView* sender) OVERRIDE;
  virtual views::MenuItemView* GetSiblingMenu(
      views::MenuItemView* menu,
      const gfx::Point& screen_point,
      views::MenuItemView::AnchorPosition* anchor,
      bool* has_mnemonics,
      views::MenuButton** button) OVERRIDE;
  virtual int GetMaxWidthForMenu(views::MenuItemView* view) OVERRIDE;

  
  virtual void BookmarkModelChanged() OVERRIDE;

 private:
  
  virtual ~BookmarkMenuController();

  scoped_ptr<views::MenuRunner> menu_runner_;

  scoped_ptr<BookmarkMenuDelegate> menu_delegate_;

  
  const BookmarkNode* node_;

  
  BookmarkNodeData drop_data_;

  
  BookmarkMenuControllerObserver* observer_;

  
  bool for_drop_;

  
  
  
  BookmarkBarView* bookmark_bar_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkMenuController);
};

#endif  
