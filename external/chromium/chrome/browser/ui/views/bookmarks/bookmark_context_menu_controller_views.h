// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_CONTROLLER_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_CONTROLLER_VIEWS_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "ui/gfx/native_widget_types.h"

class PageNavigator;
class Profile;

class BookmarkContextMenuControllerViewsDelegate {
 public:
  virtual ~BookmarkContextMenuControllerViewsDelegate() {}

  
  virtual void CloseMenu() = 0;

  
  virtual void AddItemWithStringId(int command_id, int string_id) = 0;
  virtual void AddSeparator() = 0;
  virtual void AddCheckboxItem(int command_id, int string_id) = 0;

  
  virtual void WillRemoveBookmarks(
      const std::vector<const BookmarkNode*>& bookmarks) {}

  
  virtual void DidRemoveBookmarks() {}
};

class BookmarkContextMenuControllerViews : public BaseBookmarkModelObserver {
 public:
  
  
  
  
  
  
  
  BookmarkContextMenuControllerViews(
      gfx::NativeWindow parent_window,
      BookmarkContextMenuControllerViewsDelegate* delegate,
      Profile* profile,
      PageNavigator* navigator,
      const BookmarkNode* parent,
      const std::vector<const BookmarkNode*>& selection);
  virtual ~BookmarkContextMenuControllerViews();

  void BuildMenu();

  void ExecuteCommand(int id);
  bool IsItemChecked(int id) const;
  bool IsCommandEnabled(int id) const;

  
  Profile* profile() const { return profile_; }
  PageNavigator* navigator() const { return navigator_; }

 private:
  
  
  virtual void BookmarkModelChanged();

  
  BookmarkModel* RemoveModelObserver();

  
  bool HasURLs() const;

  gfx::NativeWindow parent_window_;
  BookmarkContextMenuControllerViewsDelegate* delegate_;
  Profile* profile_;
  PageNavigator* navigator_;
  const BookmarkNode* parent_;
  std::vector<const BookmarkNode*> selection_;
  BookmarkModel* model_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkContextMenuControllerViews);
};

#endif  
