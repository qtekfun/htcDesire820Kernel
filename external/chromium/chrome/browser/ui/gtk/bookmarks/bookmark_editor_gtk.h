// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_EDITOR_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_EDITOR_GTK_H_
#pragma once

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/bookmarks/bookmark_editor.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "ui/base/gtk/gtk_integers.h"
#include "ui/base/gtk/gtk_signal.h"

class GURL;

typedef union  _GdkEvent GdkEvent;
typedef struct _GdkEventButton GdkEventButton;
typedef struct _GtkTreeIter GtkTreeIter;
typedef struct _GtkTreeSelection GtkTreeSelection;
typedef struct _GtkTreeStore GtkTreeStore;
typedef struct _GtkWidget GtkWidget;

namespace gfx {
class Point;
}  

class BookmarkEditorGtk : public BookmarkEditor,
                          public BookmarkModelObserver {
 public:
  BookmarkEditorGtk(GtkWindow* window,
                    Profile* profile,
                    const BookmarkNode* parent,
                    const EditDetails& details,
                    BookmarkEditor::Configuration configuration);

  virtual ~BookmarkEditorGtk();

  void Show();
  void Close();

 private:
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, ChangeParent);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, ChangeParentAndURL);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, ChangeURLToExistingURL);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, EditTitleKeepsPosition);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, EditURLKeepsPosition);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, ModelsMatch);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, MoveToNewParent);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, NewURL);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, ChangeURLNoTree);
  FRIEND_TEST_ALL_PREFIXES(BookmarkEditorGtkTest, ChangeTitleNoTree);

  class ContextMenuController;
  friend class ContextMenuController;

  void Init(GtkWindow* parent_window);

  
  
  virtual void Loaded(BookmarkModel* model) { }
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
                                   const BookmarkNode* node) {}
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node);
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node) {}

  
  void Reset();

  
  GURL GetInputURL() const;

  
  string16 GetInputTitle() const;

  
  
  
  
  void ApplyEdits();

  
  
  void ApplyEdits(GtkTreeIter* selected_parent);

  
  
  void AddNewFolder(GtkTreeIter* parent, GtkTreeIter* child);

  CHROMEGTK_CALLBACK_0(BookmarkEditorGtk, void, OnSelectionChanged);
  CHROMEGTK_CALLBACK_1(BookmarkEditorGtk, void, OnResponse, int);
  CHROMEGTK_CALLBACK_1(BookmarkEditorGtk, gboolean, OnWindowDeleteEvent,
                       GdkEvent*);

  CHROMEGTK_CALLBACK_0(BookmarkEditorGtk, void, OnWindowDestroy);
  CHROMEGTK_CALLBACK_0(BookmarkEditorGtk, void, OnEntryChanged);

  CHROMEGTK_CALLBACK_0(BookmarkEditorGtk, void, OnNewFolderClicked);

  CHROMEGTK_CALLBACK_1(BookmarkEditorGtk, gboolean, OnTreeViewButtonPressEvent,
                       GdkEventButton*);

  void NewFolder();

  
  Profile* profile_;

  
  GtkWidget* dialog_;
  GtkWidget* name_entry_;
  GtkWidget* url_entry_;  
  GtkWidget* tree_view_;
  GtkWidget* new_folder_button_;

  
  GtkTreeSelection* tree_selection_;

  
  
  
  GtkTreeStore* tree_store_;

  
  

  
  const BookmarkNode* parent_;

  
  const EditDetails details_;

  
  BookmarkModel* bb_model_;

  
  
  bool running_menu_for_root_;

  
  bool show_tree_;

  
  scoped_ptr<ContextMenuController> menu_controller_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkEditorGtk);
};

#endif  
