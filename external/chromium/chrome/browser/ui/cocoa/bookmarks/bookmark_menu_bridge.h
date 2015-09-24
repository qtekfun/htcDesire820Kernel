// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MENU_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MENU_BRIDGE_H_
#pragma once

#include <map>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"

class BookmarkNode;
class Profile;
@class NSImage;
@class NSMenu;
@class NSMenuItem;
@class BookmarkMenuCocoaController;

class BookmarkMenuBridge : public BookmarkModelObserver {
 public:
  BookmarkMenuBridge(Profile* profile);
  virtual ~BookmarkMenuBridge();

  
  virtual void Loaded(BookmarkModel* model);
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model);
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index);
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index);
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node);
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node);
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node);
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node);

  
  void UpdateMenu(NSMenu* bookmark_menu);

  
  BookmarkModel* GetBookmarkModel();
  Profile* GetProfile();

 protected:
  
  void ClearBookmarkMenu(NSMenu* menu);

  
  void InvalidateMenu()  { menuIsValid_ = false; }

  
  
  void AddNodeAsSubmenu(NSMenu* menu,
                        const BookmarkNode* node,
                        NSString* title);

  
  
  
  void AddNodeToMenu(const BookmarkNode* node, NSMenu* menu);

  
  
  
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

  
  scoped_nsobject<NSImage> folder_image_;

  
  
  std::map<const BookmarkNode*, NSMenuItem*> bookmark_nodes_;
};

#endif  
