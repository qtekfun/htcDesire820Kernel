// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_SUB_MENU_MODEL_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_SUB_MENU_MODEL_GTK_H_

#include <vector>

#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/window_open_disposition.h"

class Browser;
class BookmarkModel;
class BookmarkNode;
class MenuGtk;  
class Profile;

namespace content {
class PageNavigator;
}

class BookmarkNodeMenuModel : public ui::SimpleMenuModel {
 public:
  BookmarkNodeMenuModel(ui::SimpleMenuModel::Delegate* delegate,
                        BookmarkModel* model,
                        const BookmarkNode* node,
                        content::PageNavigator* page_navigator,
                        Profile* profile);
  virtual ~BookmarkNodeMenuModel();

  
  
  void Clear();

  
  virtual void MenuWillShow() OVERRIDE;
  virtual void MenuClosed() OVERRIDE;
  virtual void ActivatedAt(int index) OVERRIDE;
  virtual void ActivatedAt(int index, int event_flags) OVERRIDE;

 protected:
  
  void PopulateMenu();

  
  void AddSubMenuForNode(const BookmarkNode* node);

  BookmarkModel* model() const { return model_; }
  void set_model(BookmarkModel* model) { model_ = model; }

  const BookmarkNode* node() const { return node_; }
  void set_node(const BookmarkNode* node) { node_ = node; }

 private:
  
  void NavigateToMenuItem(int index, WindowOpenDisposition disposition);

  
  
  BookmarkModel* model_;

  
  
  const BookmarkNode* node_;

  
  content::PageNavigator* page_navigator_;

  Profile* profile_;

  
  std::vector<BookmarkNodeMenuModel*> submenus_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkNodeMenuModel);
};

class BookmarkSubMenuModel : public BookmarkNodeMenuModel,
                             public BaseBookmarkModelObserver {
 public:
  BookmarkSubMenuModel(ui::SimpleMenuModel::Delegate* delegate,
                       Browser* browser);

  virtual ~BookmarkSubMenuModel();

  
  void SetMenuGtk(MenuGtk* menu) { menu_ = menu; }

  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;

  
  virtual void MenuWillShow() OVERRIDE;
  virtual void MenuClosed() OVERRIDE;
  virtual void ActivatedAt(int index) OVERRIDE;
  virtual void ActivatedAt(int index, int event_flags) OVERRIDE;
  virtual bool IsEnabledAt(int index) const OVERRIDE;
  virtual bool IsVisibleAt(int index) const OVERRIDE;

  
  static bool IsBookmarkItemCommandId(int command_id);

 private:
  Browser* browser_;

  
  int fixed_items_;
  
  int bookmark_end_;

  
  
  
  MenuGtk* menu_;

  
  
  
  bool menu_showing_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkSubMenuModel);
};

#endif  
