// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BUBBLE_GTK_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/ui/gtk/info_bubble_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "ui/base/gtk/gtk_signal.h"

class BookmarkNode;
class Profile;
class RecentlyUsedFoldersComboModel;

typedef struct _GtkWidget GtkWidget;
typedef struct _GParamSpec GParamSpec;

class BookmarkBubbleGtk : public InfoBubbleGtkDelegate,
                          public NotificationObserver {
 public:
  
  static void Show(GtkWidget* anchor_widget,
                   Profile* profile,
                   const GURL& url,
                   bool newly_bookmarked);

  
  
  
  virtual void InfoBubbleClosing(InfoBubbleGtk* info_bubble,
                                 bool closed_by_escape);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  BookmarkBubbleGtk(GtkWidget* anchor,
                    Profile* profile,
                    const GURL& url,
                    bool newly_bookmarked);
  ~BookmarkBubbleGtk();

  
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnDestroy);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnNameActivate);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnFolderChanged);
  CHROMEGTK_CALLBACK_1(BookmarkBubbleGtk, void, OnFolderPopupShown,
                       GParamSpec*);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnEditClicked);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnCloseClicked);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnRemoveClicked);

  
  void ApplyEdits();

  
  void ShowEditor();

  
  std::string GetTitle();

  void InitFolderComboModel();

  
  GURL url_;
  
  Profile* profile_;

  
  GtkThemeService* theme_service_;

  
  GtkWidget* anchor_;

  
  
  GtkWidget* content_;

  
  GtkWidget* remove_button_;

  
  
  std::vector<GtkWidget*> labels_;

  
  GtkWidget* name_entry_;

  
  GtkWidget* folder_combo_;
  scoped_ptr<RecentlyUsedFoldersComboModel> folder_combo_model_;

  InfoBubbleGtk* bubble_;

  
  
  ScopedRunnableMethodFactory<BookmarkBubbleGtk> factory_;

  
  bool newly_bookmarked_;
  
  bool apply_edits_;
  bool remove_bookmark_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBubbleGtk);
};

#endif  
