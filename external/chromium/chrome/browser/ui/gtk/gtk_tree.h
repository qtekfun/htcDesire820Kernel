// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_TREE_H_
#define CHROME_BROWSER_UI_GTK_GTK_TREE_H_
#pragma once

#include <gtk/gtk.h>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/remove_rows_table_model.h"
#include "ui/base/models/table_model_observer.h"
#include "ui/base/models/tree_model.h"

namespace ui {
class TableModel;
}

namespace gtk_tree {

gint GetRowNumForPath(GtkTreePath* path);

gint GetRowNumForIter(GtkTreeModel* model, GtkTreeIter* iter);

gint GetTreeSortChildRowNumForPath(GtkTreeModel* sort_model,
                                   GtkTreePath* sort_path);

void SelectAndFocusRowNum(int row, GtkTreeView* tree_view);

bool RemoveRecursively(GtkTreeStore* tree_store, GtkTreeIter* iter);

void GetSelectedIndices(GtkTreeSelection* selection, std::set<int>* out);

class TableAdapter : public ui::TableModelObserver {
 public:

  enum ColumnID {
    COL_TITLE = 0,
    COL_IS_HEADER,
    COL_IS_SEPARATOR,
    COL_GROUP_ID,
    COL_WEIGHT,
    COL_WEIGHT_SET,
    COL_LAST_ID
  };

  class Delegate {
   public:
    
    virtual void SetColumnValues(int row, GtkTreeIter* iter) = 0;

    
    
    virtual void OnAnyModelUpdateStart() {}

    
    virtual void OnAnyModelUpdate() {}

    
    
    
    
    virtual void OnModelChanged() {}

   protected:
    virtual ~Delegate() {}
  };

  
  TableAdapter(Delegate* delegate,
               GtkListStore* list_store,
               ui::TableModel* table_model);
  virtual ~TableAdapter() {}

  
  
  
  
  void SetModel(ui::TableModel* table_model);

  
  void MapListStoreIndicesToModelRows(const std::set<int>& list_store_indices,
                                      RemoveRowsTableModel::Rows* model_rows);

  
  
  static gboolean OnCheckRowIsSeparator(GtkTreeModel* model,
                                        GtkTreeIter* iter,
                                        gpointer user_data);

  
  
  static gboolean OnSelectionFilter(GtkTreeSelection* selection,
                                    GtkTreeModel* model,
                                    GtkTreePath* path,
                                    gboolean path_currently_selected,
                                    gpointer user_data);

  
  virtual void OnModelChanged();
  virtual void OnItemsChanged(int start, int length);
  virtual void OnItemsAdded(int start, int length);
  virtual void OnItemsRemoved(int start, int length);

 private:
  
  
  bool IsGroupRow(GtkTreeIter* iter) const;

  
  int GetListStoreIndexForModelRow(int model_row) const;

  
  void AddNodeToList(int row);

  Delegate* delegate_;
  GtkListStore* list_store_;
  ui::TableModel* table_model_;

  DISALLOW_COPY_AND_ASSIGN(TableAdapter);
};

class TreeAdapter : public ui::TreeModelObserver {
 public:
  
  enum {
    COL_ICON,
    COL_TITLE,
    COL_NODE_PTR,
    COL_COUNT,
  };

  class Delegate {
   public:
    
    
    virtual void OnAnyModelUpdateStart() {}

    
    virtual void OnAnyModelUpdate() {}

   protected:
    virtual ~Delegate() {}
  };

  TreeAdapter(Delegate* delegate, ui::TreeModel* tree_model);
  virtual ~TreeAdapter();

  
  void Init();

  
  GtkTreeStore* tree_store() { return tree_store_; }

  
  ui::TreeModelNode* GetNode(GtkTreeIter* iter);

  
  virtual void TreeNodesAdded(ui::TreeModel* model,
                              ui::TreeModelNode* parent,
                              int start,
                              int count);
  virtual void TreeNodesRemoved(ui::TreeModel* model,
                                ui::TreeModelNode* parent,
                                int start,
                                int count);
  virtual void TreeNodeChanged(ui::TreeModel* model, ui::TreeModelNode* node);
  

 private:
  
  void FillRow(GtkTreeIter* iter, ui::TreeModelNode* node);

  
  void Fill(GtkTreeIter* parent_iter, ui::TreeModelNode* parent_node);

  
  
  GtkTreePath* GetTreePath(ui::TreeModelNode* node);

  
  bool GetTreeIter(ui::TreeModelNode* node, GtkTreeIter* iter);

  Delegate* delegate_;
  GtkTreeStore* tree_store_;
  ui::TreeModel* tree_model_;
  std::vector<GdkPixbuf*> pixbufs_;

  DISALLOW_COPY_AND_ASSIGN(TreeAdapter);
};

}  

#endif  
