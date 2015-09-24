// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_MENU_CONTROLLER_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_MENU_CONTROLLER_VIEWS_H_
#pragma once

#include <map>
#include <set>

#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_context_menu.h"
#include "ui/gfx/native_widget_types.h"
#include "views/controls/menu/menu_delegate.h"
#include "views/controls/menu/menu_item_view.h"

class BookmarkBarView;
class BookmarkNode;
class Browser;
class PageNavigator;
class Profile;

namespace gfx {
class Rect;
}  

namespace ui {
class OSExchangeData;
}  

namespace views {
class MenuButton;
}  

class BookmarkMenuController : public BaseBookmarkModelObserver,
                               public views::MenuDelegate,
                               public BookmarkContextMenuObserver {
 public:
  
  class Observer {
   public:
    virtual void BookmarkMenuDeleted(BookmarkMenuController* controller) = 0;

   protected:
    virtual ~Observer() {}
  };

  
  
  BookmarkMenuController(Browser* browser,
                         Profile* profile,
                         PageNavigator* page_navigator,
                         gfx::NativeWindow parent,
                         const BookmarkNode* node,
                         int start_child_index);

  void RunMenuAt(BookmarkBarView* bookmark_bar, bool for_drop);

  
  void RunMenuAt(views::MenuButton* button,
                 views::MenuItemView::AnchorPosition position,
                 bool for_drop);

  
  void Cancel();

  
  const BookmarkNode* node() const { return node_; }

  
  views::MenuItemView* menu() const { return menu_; }

  
  views::MenuItemView* context_menu() const {
    return context_menu_.get() ? context_menu_->menu() : NULL;
  }

  void set_observer(Observer* observer) { observer_ = observer; }

  
  virtual std::wstring GetTooltipText(int id, const gfx::Point& p);
  virtual bool IsTriggerableEvent(const views::MouseEvent& e);
  virtual void ExecuteCommand(int id, int mouse_event_flags);
  virtual bool GetDropFormats(
      views::MenuItemView* menu,
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats);
  virtual bool AreDropTypesRequired(views::MenuItemView* menu);
  virtual bool CanDrop(views::MenuItemView* menu,
                       const ui::OSExchangeData& data);
  virtual int GetDropOperation(views::MenuItemView* item,
                               const views::DropTargetEvent& event,
                               DropPosition* position);
  virtual int OnPerformDrop(views::MenuItemView* menu,
                            DropPosition position,
                            const views::DropTargetEvent& event);
  virtual bool ShowContextMenu(views::MenuItemView* source,
                               int id,
                               const gfx::Point& p,
                               bool is_mouse_gesture);
  virtual void DropMenuClosed(views::MenuItemView* menu);
  virtual bool CanDrag(views::MenuItemView* menu);
  virtual void WriteDragData(views::MenuItemView* sender,
                             ui::OSExchangeData* data);
  virtual int GetDragOperations(views::MenuItemView* sender);
  virtual views::MenuItemView* GetSiblingMenu(
      views::MenuItemView* menu,
      const gfx::Point& screen_point,
      views::MenuItemView::AnchorPosition* anchor,
      bool* has_mnemonics,
      views::MenuButton** button);
  virtual int GetMaxWidthForMenu();

  
  virtual void BookmarkModelChanged();
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node);

  
  virtual void WillRemoveBookmarks(
      const std::vector<const BookmarkNode*>& bookmarks);
  virtual void DidRemoveBookmarks();

 private:
  typedef std::map<const BookmarkNode*, int> NodeToMenuIDMap;

  
  virtual ~BookmarkMenuController();

  
  
  views::MenuItemView* CreateMenu(const BookmarkNode* parent,
                                  int start_child_index);

  
  
  void BuildMenu(const BookmarkNode* parent,
                 int start_child_index,
                 views::MenuItemView* menu,
                 int* next_menu_id);

  
  views::MenuItemView* GetMenuByID(int id);

  
  
  
  void WillRemoveBookmarksImpl(
      const std::vector<const BookmarkNode*>& bookmarks,
      std::set<views::MenuItemView*>* removed_menus);

  Browser* browser_;

  Profile* profile_;

  PageNavigator* page_navigator_;

  
  gfx::NativeWindow parent_;

  
  const BookmarkNode* node_;

  
  std::map<int, const BookmarkNode*> menu_id_to_node_map_;

  
  
  NodeToMenuIDMap node_to_menu_id_map_;

  
  views::MenuItemView* menu_;

  
  BookmarkNodeData drop_data_;

  
  scoped_ptr<BookmarkContextMenu> context_menu_;

  
  Observer* observer_;

  
  bool for_drop_;

  
  
  BookmarkBarView* bookmark_bar_;

  typedef std::map<const BookmarkNode*, views::MenuItemView*> NodeToMenuMap;
  NodeToMenuMap node_to_menu_map_;

  
  int next_menu_id_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkMenuController);
};

#endif  
