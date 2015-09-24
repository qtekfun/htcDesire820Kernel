// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_TREE_MODEL_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_TREE_MODEL_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"

class BookmarkModel;
class BookmarkNode;

typedef struct _GtkCellRenderer GtkCellRenderer;
typedef struct _GtkTreeIter GtkTreeIter;
typedef struct _GtkTreeModel GtkTreeModel;
typedef struct _GtkTreeStore GtkTreeStore;
typedef struct _GtkTreeView GtkTreeView;
typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GtkWidget GtkWidget;

enum FolderTreeStoreColumns {
  FOLDER_ICON,
  FOLDER_NAME,
  ITEM_ID,
  IS_EDITABLE,
  FOLDER_STORE_NUM_COLUMNS
};

GtkTreeStore* MakeFolderTreeStore();

void AddToTreeStore(BookmarkModel* model, int64 selected_id,
                    GtkTreeStore* store, GtkTreeIter* selected_iter);

void AddToTreeStoreAt(const BookmarkNode* node, int64 selected_id,
                      GtkTreeStore* store, GtkTreeIter* selected_iter,
                      GtkTreeIter* parent);

GtkWidget* MakeTreeViewForStore(GtkTreeStore* store);

GtkCellRenderer* GetCellRendererText(GtkTreeView* tree_view);

const BookmarkNode* CommitTreeStoreDifferencesBetween(
    BookmarkModel* model, GtkTreeStore* tree_store,
    GtkTreeIter* selected);

int64 GetIdFromTreeIter(GtkTreeModel* model, GtkTreeIter* iter);

base::string16 GetTitleFromTreeIter(GtkTreeModel* model, GtkTreeIter* iter);

#endif  
