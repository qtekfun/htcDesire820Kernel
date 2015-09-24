// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_COLLECTED_COOKIES_GTK_H_
#define CHROME_BROWSER_UI_GTK_COLLECTED_COOKIES_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/constrained_window_gtk.h"
#include "chrome/browser/ui/gtk/gtk_tree.h"
#include "chrome/common/content_settings.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class CookiesTreeModel;


class CollectedCookiesGtk : public ConstrainedDialogDelegate,
                                   gtk_tree::TreeAdapter::Delegate,
                                   NotificationObserver {
 public:
  CollectedCookiesGtk(GtkWindow* parent, TabContents* tab_contents);

  
  virtual GtkWidget* GetWidgetRoot();
  virtual GtkWidget* GetFocusWidget();
  virtual void DeleteDelegate();

 private:
  virtual ~CollectedCookiesGtk();

  
  void Init();

  
  bool SelectionContainsOriginNode(GtkTreeSelection* selection,
                                   gtk_tree::TreeAdapter* adapter);

  
  void EnableControls();

  
  void AddExceptions(GtkTreeSelection* selection,
                     gtk_tree::TreeAdapter* adapter,
                     ContentSetting setting);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  GtkWidget* CreateAllowedPane();
  GtkWidget* CreateBlockedPane();

  
  void ShowCookieInfo(gint current_page);
  void ShowSelectionInfo(GtkTreeSelection* selection,
                         gtk_tree::TreeAdapter* adapter);


  
  CHROMEGTK_CALLBACK_2(CollectedCookiesGtk, void, OnTreeViewRowExpanded,
                       GtkTreeIter*, GtkTreePath*);
  CHROMEGTK_CALLBACK_0(CollectedCookiesGtk, void, OnTreeViewSelectionChange);
  CHROMEGTK_CALLBACK_0(CollectedCookiesGtk, void, OnClose);
  CHROMEGTK_CALLBACK_0(CollectedCookiesGtk, void, OnBlockAllowedButtonClicked);
  CHROMEGTK_CALLBACK_0(CollectedCookiesGtk, void, OnAllowBlockedButtonClicked);
  CHROMEGTK_CALLBACK_0(CollectedCookiesGtk, void,
                       OnForSessionBlockedButtonClicked);
  CHROMEGTK_CALLBACK_2(CollectedCookiesGtk, void, OnSwitchPage,
                       gpointer, guint);

  NotificationRegistrar registrar_;

  ConstrainedWindow* window_;

  
  GtkWidget* dialog_;

  GtkWidget* allowed_description_label_;
  GtkWidget* blocked_description_label_;

  GtkWidget* block_allowed_cookie_button_;

  GtkWidget* allow_blocked_cookie_button_;
  GtkWidget* for_session_blocked_cookie_button_;
  GtkWidget* close_button_;

  
  GtkWidget* notebook_;
  GtkWidget* allowed_tree_;
  GtkWidget* blocked_tree_;

  GtkTreeSelection* allowed_selection_;
  GtkTreeSelection* blocked_selection_;

  
  GtkWidget* infobar_;
  GtkWidget* infobar_label_;

  
  GtkWidget* cookie_info_view_;

  
  TabContents* tab_contents_;

  bool status_changed_;

  
  scoped_ptr<CookiesTreeModel> allowed_cookies_tree_model_;
  scoped_ptr<CookiesTreeModel> blocked_cookies_tree_model_;
  scoped_ptr<gtk_tree::TreeAdapter> allowed_cookies_tree_adapter_;
  scoped_ptr<gtk_tree::TreeAdapter> blocked_cookies_tree_adapter_;

  DISALLOW_COPY_AND_ASSIGN(CollectedCookiesGtk);
};

#endif  
