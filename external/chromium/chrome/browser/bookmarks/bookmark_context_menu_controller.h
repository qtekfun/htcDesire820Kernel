// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_CONTEXT_MENU_CONTROLLER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_CONTEXT_MENU_CONTROLLER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/native_widget_types.h"

class PageNavigator;
class Profile;

class BookmarkContextMenuControllerDelegate {
 public:
  virtual ~BookmarkContextMenuControllerDelegate() {}

  
  virtual void CloseMenu() = 0;

  
  virtual void WillExecuteCommand() {}

  
  virtual void DidExecuteCommand() {}
};

class BookmarkContextMenuController : public BaseBookmarkModelObserver,
                                      public ui::SimpleMenuModel::Delegate {
 public:
  
  
  
  
  
  
  BookmarkContextMenuController(
      gfx::NativeWindow parent_window,
      BookmarkContextMenuControllerDelegate* delegate,
      Profile* profile,
      PageNavigator* navigator,
      const BookmarkNode* parent,
      const std::vector<const BookmarkNode*>& selection);
  virtual ~BookmarkContextMenuController();

  void BuildMenu();

  ui::SimpleMenuModel* menu_model() const { return menu_model_.get(); }

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

  
  Profile* profile() const { return profile_; }
  PageNavigator* navigator() const { return navigator_; }

 private:
  
  void AddItem(int id, int localization_id);
  
  void AddSeparator();
  
  void AddCheckboxItem(int id, int localization_id);

  
  
  virtual void BookmarkModelChanged();

  
  bool HasURLs() const;

  gfx::NativeWindow parent_window_;
  BookmarkContextMenuControllerDelegate* delegate_;
  Profile* profile_;
  PageNavigator* navigator_;
  const BookmarkNode* parent_;
  std::vector<const BookmarkNode*> selection_;
  BookmarkModel* model_;
  scoped_ptr<ui::SimpleMenuModel> menu_model_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkContextMenuController);
};

#endif  
