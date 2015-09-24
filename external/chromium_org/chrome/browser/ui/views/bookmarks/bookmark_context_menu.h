// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/bookmarks/bookmark_context_menu_controller.h"
#include "ui/views/controls/menu/menu_delegate.h"

class Browser;

namespace views {
class MenuRunner;
class Widget;
}

class BookmarkContextMenuObserver {
 public:
  
  virtual void WillRemoveBookmarks(
      const std::vector<const BookmarkNode*>& bookmarks) = 0;

  
  virtual void DidRemoveBookmarks() = 0;

 protected:
  virtual ~BookmarkContextMenuObserver() {}
};

class BookmarkContextMenu : public BookmarkContextMenuControllerDelegate,
                            public views::MenuDelegate {
 public:
  
  BookmarkContextMenu(
      views::Widget* parent_widget,
      Browser* browser,
      Profile* profile,
      content::PageNavigator* page_navigator,
      const BookmarkNode* parent,
      const std::vector<const BookmarkNode*>& selection,
      bool close_on_remove);
  virtual ~BookmarkContextMenu();

  
  void RunMenuAt(const gfx::Point& point,
                 ui::MenuSourceType source_type);

  views::MenuItemView* menu() const { return menu_; }

  void set_observer(BookmarkContextMenuObserver* observer) {
    observer_ = observer;
  }

  
  void SetPageNavigator(content::PageNavigator* navigator);

  
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual bool IsItemChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandEnabled(int command_id) const OVERRIDE;
  virtual bool ShouldCloseAllMenusOnExecute(int id) OVERRIDE;

  
  virtual void CloseMenu() OVERRIDE;
  virtual void WillExecuteCommand(
      int command_id,
      const std::vector<const BookmarkNode*>& bookmarks) OVERRIDE;
  virtual void DidExecuteCommand(int command_id) OVERRIDE;

 private:
  scoped_ptr<BookmarkContextMenuController> controller_;

  
  views::Widget* parent_widget_;

  
  views::MenuItemView* menu_;

  
  scoped_ptr<views::MenuRunner> menu_runner_;

  
  const BookmarkNode* parent_node_;

  BookmarkContextMenuObserver* observer_;

  
  bool close_on_remove_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkContextMenu);
};

#endif  
