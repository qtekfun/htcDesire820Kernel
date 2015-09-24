// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_RECENTLY_USED_FOLDERS_COMBO_MODEL_H_
#define CHROME_BROWSER_UI_BOOKMARKS_RECENTLY_USED_FOLDERS_COMBO_MODEL_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "ui/base/models/combobox_model.h"

class BookmarkModel;
class BookmarkNode;

class RecentlyUsedFoldersComboModel
    : public ui::ComboboxModel,
      public BookmarkModelObserver {
 public:
  RecentlyUsedFoldersComboModel(BookmarkModel* model, const BookmarkNode* node);
  virtual ~RecentlyUsedFoldersComboModel();

  
  virtual int GetItemCount() const OVERRIDE;
  virtual base::string16 GetItemAt(int index) OVERRIDE;
  virtual bool IsItemSeparatorAt(int index) OVERRIDE;
  virtual int GetDefaultIndex() const OVERRIDE;
  virtual void AddObserver(ui::ComboboxModelObserver* observer) OVERRIDE;
  virtual void RemoveObserver(ui::ComboboxModelObserver* observer) OVERRIDE;

  
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
  virtual void OnWillRemoveBookmarks(BookmarkModel* model,
                                     const BookmarkNode* parent,
                                     int old_index,
                                     const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChildrenReordered(
      BookmarkModel* model,
      const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;

  
  
  void MaybeChangeParent(const BookmarkNode* node, int selected_index);

 private:
  
  const BookmarkNode* GetNodeAt(int index);

  
  void RemoveNode(const BookmarkNode* node);

  struct Item;
  std::vector<Item> items_;

  BookmarkModel* bookmark_model_;

  
  int node_parent_index_;

  ObserverList<ui::ComboboxModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(RecentlyUsedFoldersComboModel);
};

#endif  
