// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TASK_MANAGER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TASK_MANAGER_GTK_H_

#include <gtk/gtk.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/task_manager/task_manager.h"
#include "grit/generated_resources.h"
#include "ui/base/gtk/gtk_signal.h"

namespace gfx {
class Point;
}

class TaskManagerGtk : public TaskManagerModelObserver {
 public:
  TaskManagerGtk();
  virtual ~TaskManagerGtk();

  
  virtual void OnModelChanged() OVERRIDE;
  virtual void OnItemsChanged(int start, int length) OVERRIDE;
  virtual void OnItemsAdded(int start, int length) OVERRIDE;
  virtual void OnItemsRemoved(int start, int length) OVERRIDE;

  
  void Close();

  
  
  static void Show();

 private:
  class ContextMenuController;
  friend class ContextMenuController;

  
  void Init();

  
  void SetInitialDialogSize();

  
  void ConnectAccelerators();

  
  void CreateTaskManagerTreeview();

  
  std::string GetModelText(int row, int col_id);

  
  GdkPixbuf* GetModelIcon(int row);

  
  
  void SetRowDataFromModel(int row, GtkTreeIter* iter);

  
  void KillSelectedProcesses();

  
  void ShowContextMenu(const gfx::Point& point, guint32 event_time);

  
  void OnLinkActivated();

  
  gint CompareImpl(GtkTreeModel* tree_model, GtkTreeIter* a,
                   GtkTreeIter* b, int id);

  
  CHROMEGTK_CALLBACK_0(TaskManagerGtk, void, OnDestroy);

  
  CHROMEGTK_CALLBACK_1(TaskManagerGtk, void, OnResponse, int);

  
  CHROMEG_CALLBACK_0(TaskManagerGtk, void, OnTreeViewRealize, GtkTreeView*);

  
  CHROMEG_CALLBACK_0(TaskManagerGtk, void, OnSelectionChanged,
                     GtkTreeSelection*);

  
  
  CHROMEGTK_CALLBACK_2(TaskManagerGtk, void, OnRowActivated,
                       GtkTreePath*, GtkTreeViewColumn*);

  
  
  
  CHROMEGTK_CALLBACK_1(TaskManagerGtk, gboolean, OnButtonEvent,
                       GdkEventButton*);

  
  CHROMEG_CALLBACK_3(TaskManagerGtk, gboolean, OnGtkAccelerator,
                     GtkAccelGroup*, GObject*, guint, GdkModifierType);

  
  static gint ComparePage(GtkTreeModel* model, GtkTreeIter* a,
                          GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_TASK_COLUMN);
  }

  
  static gint CompareProfileName(GtkTreeModel* model, GtkTreeIter* a,
                                 GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_PROFILE_NAME_COLUMN);
  }

  
  static gint CompareSharedMemory(GtkTreeModel* model, GtkTreeIter* a,
                                  GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_SHARED_MEM_COLUMN);
  }

  
  static gint ComparePrivateMemory(GtkTreeModel* model, GtkTreeIter* a,
                                   GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_PRIVATE_MEM_COLUMN);
  }

  
  static gint CompareV8Memory(GtkTreeModel* model, GtkTreeIter* a,
                              GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b,
                    IDS_TASK_MANAGER_JAVASCRIPT_MEMORY_ALLOCATED_COLUMN);
  }

  
  static gint CompareCPU(GtkTreeModel* model, GtkTreeIter* a,
                         GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_CPU_COLUMN);
  }

  
  static gint CompareNetwork(GtkTreeModel* model, GtkTreeIter* a,
                             GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_NET_COLUMN);
  }

  
  static gint CompareProcessID(GtkTreeModel* model, GtkTreeIter* a,
                               GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_PROCESS_ID_COLUMN);
  }

  
  static gint CompareWebCoreImageCache(GtkTreeModel* model, GtkTreeIter* a,
                                       GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_WEBCORE_IMAGE_CACHE_COLUMN);
  }

  
  static gint CompareWebCoreScriptsCache(GtkTreeModel* model, GtkTreeIter* a,
                                         GtkTreeIter* b,
                                         gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_WEBCORE_SCRIPTS_CACHE_COLUMN);
  }

  
  static gint CompareWebCoreCssCache(GtkTreeModel* model, GtkTreeIter* a,
                                     GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_WEBCORE_CSS_CACHE_COLUMN);
  }

  
  static gint CompareVideoMemory(GtkTreeModel* model, GtkTreeIter* a,
                                 GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_VIDEO_MEMORY_COLUMN);
  }

  
  static gint CompareFPS(GtkTreeModel* model, GtkTreeIter* a,
                         GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_FPS_COLUMN);
  }

  
  static gint CompareSqliteMemoryUsed(GtkTreeModel* model, GtkTreeIter* a,
                                      GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_SQLITE_MEMORY_USED_COLUMN);
  }

  
  static gint CompareGoatsTeleported(GtkTreeModel* model, GtkTreeIter* a,
                                     GtkTreeIter* b, gpointer task_manager) {
    return reinterpret_cast<TaskManagerGtk*>(task_manager)->
        CompareImpl(model, a, b, IDS_TASK_MANAGER_GOATS_TELEPORTED_COLUMN);
  }

  
  TaskManager* task_manager_;

  
  TaskManagerModel* model_;

  
  GtkWidget* dialog_;

  
  GtkWidget* treeview_;

  
  GtkListStore* process_list_;
  GtkTreeModel* process_list_sort_;

  
  int process_count_;

  
  gulong destroy_handler_id_;

  
  scoped_ptr<ContextMenuController> menu_controller_;

  GtkAccelGroup* accel_group_;

  
  
  static TaskManagerGtk* instance_;

  
  
  
  bool ignore_selection_changed_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerGtk);
};

#endif  
