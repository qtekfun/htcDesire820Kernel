// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_MENU_CONTROLLER_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_MENU_CONTROLLER_GTK_H_
#pragma once

#include <map>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_context_menu_controller.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "ui/base/gtk/gtk_integers.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "webkit/glue/window_open_disposition.h"

class Browser;
class Profile;
class Profiler;
class PageNavigator;
class BookmarkModel;
class BookmarkNode;
class MenuGtk;

typedef struct _GdkDragContext GdkDragContext;
typedef struct _GdkEventButton GdkEventButton;
typedef struct _GtkSelectionData GtkSelectionData;
typedef struct _GtkWidget GtkWidget;

class BookmarkMenuController : public BaseBookmarkModelObserver,
                               public BookmarkContextMenuControllerDelegate {
 public:
  
  
  BookmarkMenuController(Browser* browser,
                         Profile* profile,
                         PageNavigator* page_navigator,
                         GtkWindow* window,
                         const BookmarkNode* node,
                         int start_child_index);
  virtual ~BookmarkMenuController();

  GtkWidget* widget() { return menu_; }

  
  void Popup(GtkWidget* widget, gint button_type, guint32 timestamp);

  
  virtual void BookmarkModelChanged();
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node);

  
  virtual void WillExecuteCommand();
  virtual void CloseMenu();

 private:
  
  
  void BuildMenu(const BookmarkNode* parent,
                 int start_child_index,
                 GtkWidget* menu);

  
  
  void NavigateToMenuItem(GtkWidget* menu_item,
                          WindowOpenDisposition disposition);

  
  CHROMEGTK_CALLBACK_1(BookmarkMenuController, gboolean,
                       OnMenuButtonPressedOrReleased, GdkEventButton*);

  
  CHROMEGTK_CALLBACK_1(BookmarkMenuController, gboolean, OnButtonReleased,
                       GdkEventButton*);

  
  
  
  
  CHROMEGTK_CALLBACK_1(BookmarkMenuController, gboolean, OnFolderButtonPressed,
                       GdkEventButton*);

  
  
  CHROMEGTK_CALLBACK_0(BookmarkMenuController, void, OnMenuHidden)

  
  
  CHROMEGTK_CALLBACK_0(BookmarkMenuController, void, OnMenuItemActivated);

  
  CHROMEGTK_CALLBACK_1(BookmarkMenuController, void, OnMenuItemDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(BookmarkMenuController, void, OnMenuItemDragEnd,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_4(BookmarkMenuController, void, OnMenuItemDragGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);

  Browser* browser_;
  Profile* profile_;
  PageNavigator* page_navigator_;

  
  GtkWindow* parent_window_;

  
  BookmarkModel* model_;

  
  const BookmarkNode* node_;

  
  
  
  
  GtkWidget* menu_;

  
  GtkWidget* drag_icon_;

  
  
  bool ignore_button_release_;

  
  GtkWidget* triggering_widget_;

  
  
  std::map<const BookmarkNode*, GtkWidget*> node_to_menu_widget_map_;

  
  scoped_ptr<BookmarkContextMenuController> context_menu_controller_;
  scoped_ptr<MenuGtk> context_menu_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkMenuController);
};

#endif  
