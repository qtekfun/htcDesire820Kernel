// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_CONTEXT_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_CONTEXT_MENU_CONTROLLER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class Profile;

namespace content {
class PageNavigator;
}

class BookmarkContextMenuControllerDelegate {
 public:
  virtual ~BookmarkContextMenuControllerDelegate() {}

  
  virtual void CloseMenu() = 0;

  
  virtual void WillExecuteCommand(
      int command_id,
      const std::vector<const BookmarkNode*>& bookmarks) {}

  
  virtual void DidExecuteCommand(int command_id) {}
};

class BookmarkContextMenuController : public BaseBookmarkModelObserver,
                                      public ui::SimpleMenuModel::Delegate {
 public:
  
  
  
  
  
  
  BookmarkContextMenuController(
      gfx::NativeWindow parent_window,
      BookmarkContextMenuControllerDelegate* delegate,
      Browser* browser,
      Profile* profile,
      content::PageNavigator* navigator,
      const BookmarkNode* parent,
      const std::vector<const BookmarkNode*>& selection);
  virtual ~BookmarkContextMenuController();

  ui::SimpleMenuModel* menu_model() { return menu_model_.get(); }

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  void set_navigator(content::PageNavigator* navigator) {
    navigator_ = navigator;
  }

 private:
  
  
  
  bool IsPlatformCommandIdEnabled(int command_id, bool* enabled) const;

  
  
  bool ExecutePlatformCommand(int command_id, int event_flags);

  void BuildMenu();

  
  void AddItem(int id, int localization_id);
  
  void AddSeparator();
  
  void AddCheckboxItem(int id, int localization_id);

  
  
  virtual void BookmarkModelChanged() OVERRIDE;

  gfx::NativeWindow parent_window_;
  BookmarkContextMenuControllerDelegate* delegate_;
  Browser* browser_;
  Profile* profile_;
  content::PageNavigator* navigator_;
  const BookmarkNode* parent_;
  std::vector<const BookmarkNode*> selection_;
  BookmarkModel* model_;
  scoped_ptr<ui::SimpleMenuModel> menu_model_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkContextMenuController);
};

#endif  
