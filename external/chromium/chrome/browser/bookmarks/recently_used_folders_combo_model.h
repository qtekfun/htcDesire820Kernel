// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_RECENTLY_USED_FOLDERS_COMBO_MODEL_H_
#define CHROME_BROWSER_BOOKMARKS_RECENTLY_USED_FOLDERS_COMBO_MODEL_H_
#pragma once

#include <vector>

#include "base/string16.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "ui/base/models/combobox_model.h"

class RecentlyUsedFoldersComboModel : public ui::ComboboxModel {
 public:
  RecentlyUsedFoldersComboModel(BookmarkModel* model, const BookmarkNode* node);
  virtual ~RecentlyUsedFoldersComboModel();

  
  virtual int GetItemCount();
  virtual string16 GetItemAt(int index);

  
  const BookmarkNode* GetNodeAt(int index);

  
  int node_parent_index() const { return node_parent_index_; }

 private:
  
  void RemoveNode(const BookmarkNode* node);

  std::vector<const BookmarkNode*> nodes_;
  int node_parent_index_;

  DISALLOW_COPY_AND_ASSIGN(RecentlyUsedFoldersComboModel);
};

#endif  
