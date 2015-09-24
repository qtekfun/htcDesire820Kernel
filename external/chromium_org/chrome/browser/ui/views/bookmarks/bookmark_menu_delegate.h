// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_MENU_DELEGATE_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_MENU_DELEGATE_H_

#include <map>
#include <set>

#include "base/compiler_specific.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "chrome/browser/bookmarks/bookmark_stats.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_context_menu.h"
#include "ui/views/controls/menu/menu_delegate.h"

class BookmarkNode;
class Browser;
class Profile;

namespace content {
class PageNavigator;
}

namespace ui {
class OSExchangeData;
}

namespace views {
class MenuItemView;
class Widget;
}

class BookmarkMenuDelegate : public BaseBookmarkModelObserver,
                             public BookmarkContextMenuObserver {
 public:
  enum ShowOptions {
    
    
    
    SHOW_PERMANENT_FOLDERS,

    
    HIDE_PERMANENT_FOLDERS
  };

  BookmarkMenuDelegate(Browser* browser,
                       content::PageNavigator* navigator,
                       views::Widget* parent,
                       int first_menu_id,
                       int max_menu_id);
  virtual ~BookmarkMenuDelegate();

  
  void Init(views::MenuDelegate* real_delegate,
            views::MenuItemView* parent,
            const BookmarkNode* node,
            int start_child_index,
            ShowOptions show_options,
            BookmarkLaunchLocation location);

  
  void SetPageNavigator(content::PageNavigator* navigator);

  
  int next_menu_id() const { return next_menu_id_; }

  
  
  void SetActiveMenu(const BookmarkNode* node, int start_index);

  BookmarkModel* GetBookmarkModel();

  
  views::MenuItemView* menu() { return menu_; }

  
  views::MenuItemView* context_menu() {
    return context_menu_.get() ? context_menu_->menu() : NULL;
  }

  views::Widget* parent() { return parent_; }
  const views::Widget* parent() const { return parent_; }

  
  
  bool is_mutating_model() const { return is_mutating_model_; }

  
  base::string16 GetTooltipText(int id, const gfx::Point& p) const;
  bool IsTriggerableEvent(views::MenuItemView* menu,
                          const ui::Event& e);
  void ExecuteCommand(int id, int mouse_event_flags);
  bool ShouldExecuteCommandWithoutClosingMenu(int id, const ui::Event& e);
  bool GetDropFormats(
      views::MenuItemView* menu,
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats);
  bool AreDropTypesRequired(views::MenuItemView* menu);
  bool CanDrop(views::MenuItemView* menu, const ui::OSExchangeData& data);
  int GetDropOperation(views::MenuItemView* item,
                       const ui::DropTargetEvent& event,
                       views::MenuDelegate::DropPosition* position);
  int OnPerformDrop(views::MenuItemView* menu,
                    views::MenuDelegate::DropPosition position,
                    const ui::DropTargetEvent& event);
  bool ShowContextMenu(views::MenuItemView* source,
                       int id,
                       const gfx::Point& p,
                       ui::MenuSourceType source_type);
  bool CanDrag(views::MenuItemView* menu);
  void WriteDragData(views::MenuItemView* sender, ui::OSExchangeData* data);
  int GetDragOperations(views::MenuItemView* sender);
  int GetMaxWidthForMenu(views::MenuItemView* menu);

  
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;

  
  virtual void WillRemoveBookmarks(
      const std::vector<const BookmarkNode*>& bookmarks) OVERRIDE;
  virtual void DidRemoveBookmarks() OVERRIDE;

 private:
  typedef std::map<int, const BookmarkNode*> MenuIDToNodeMap;
  typedef std::map<const BookmarkNode*, views::MenuItemView*> NodeToMenuMap;

  
  views::MenuItemView* CreateMenu(const BookmarkNode* parent,
                                  int start_child_index,
                                  ShowOptions show_options);

  
  
  void BuildMenusForPermanentNodes(views::MenuItemView* menu,
                                   int* next_menu_id);

  
  
  
  
  void BuildMenuForPermanentNode(const BookmarkNode* node,
                                 views::MenuItemView* menu,
                                 int* next_menu_id,
                                 bool* added_separator);

  
  
  void BuildMenu(const BookmarkNode* parent,
                 int start_child_index,
                 views::MenuItemView* menu,
                 int* next_menu_id);

  
  
  bool IsOutsideMenuIdRange(int menu_id) const;

  Browser* browser_;
  Profile* profile_;

  content::PageNavigator* page_navigator_;

  
  views::Widget* parent_;

  
  MenuIDToNodeMap menu_id_to_node_map_;

  
  views::MenuItemView* menu_;

  
  BookmarkNodeData drop_data_;

  
  scoped_ptr<BookmarkContextMenu> context_menu_;

  
  bool for_drop_;

  
  views::MenuItemView* parent_menu_item_;

  
  NodeToMenuMap node_to_menu_map_;

  
  int next_menu_id_;

  
  const int min_menu_id_;
  const int max_menu_id_;

  views::MenuDelegate* real_delegate_;

  
  bool is_mutating_model_;

  
  BookmarkLaunchLocation location_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkMenuDelegate);
};

#endif  
