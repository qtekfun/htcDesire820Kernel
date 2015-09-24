// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MENU_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MENU_BRIDGE_H_

#include <map>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#import "chrome/browser/ui/cocoa/main_menu_item.h"

class BookmarkNode;
class Profile;
@class NSImage;
@class NSMenu;
@class NSMenuItem;
@class BookmarkMenuCocoaController;

class BookmarkMenuBridge : public BookmarkModelObserver,
                           public MainMenuItem {
 public:
  BookmarkMenuBridge(Profile* profile, NSMenu* menu);
  virtual ~BookmarkMenuBridge();

  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) OVERRIDE;
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;

  
  virtual void ResetMenu() OVERRIDE;
  virtual void BuildMenu() OVERRIDE;

  
  void UpdateMenu(NSMenu* bookmark_menu);

  
  void UpdateSubMenu(NSMenu* bookmark_menu);

  
  BookmarkModel* GetBookmarkModel();
  Profile* GetProfile();

 protected:
  
  void UpdateMenuInternal(NSMenu* bookmark_menu, bool is_submenu);

  
  void ClearBookmarkMenu(NSMenu* menu);

  
  void InvalidateMenu()  { menuIsValid_ = false; }

  
  
  
  
  void AddNodeAsSubmenu(NSMenu* menu,
                        const BookmarkNode* node,
                        bool add_extra_items);

  
  
  
  
  
  void AddNodeToMenu(const BookmarkNode* node, NSMenu* menu,
                     bool add_extra_items);

  
  
  
  void AddItemToMenu(int command_id,
                     int message_id,
                     const BookmarkNode* node,
                     NSMenu* menu,
                     bool enabled);

  
  
  
  
  
  
  void ConfigureMenuItem(const BookmarkNode* node, NSMenuItem* item,
                         bool set_title);

  
  NSMenuItem* MenuItemForNode(const BookmarkNode* node);

  
  virtual NSMenu* BookmarkMenu();

  
  void ObserveBookmarkModel();

 private:
  friend class BookmarkMenuBridgeTest;

  
  bool menuIsValid_;

  Profile* profile_;  
  BookmarkMenuCocoaController* controller_;  

  
  base::scoped_nsobject<NSImage> folder_image_;

  
  
  std::map<const BookmarkNode*, NSMenuItem*> bookmark_nodes_;
};

#endif  
