// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BUBBLE_GTK_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "url/gurl.h"

class BookmarkModel;
class Profile;
class RecentlyUsedFoldersComboModel;

typedef struct _GtkWidget GtkWidget;
typedef struct _GParamSpec GParamSpec;

class BookmarkBubbleGtk : public BubbleDelegateGtk,
                          public content::NotificationObserver {
 public:
  
  static void Show(GtkWidget* anchor_widget,
                   Profile* profile,
                   const GURL& url,
                   bool newly_bookmarked);

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend class BookmarkBubbleGtkBrowserTest;
  FRIEND_TEST_ALL_PREFIXES(BookmarkBubbleGtkBrowserTest, SyncPromoSignedIn);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBubbleGtkBrowserTest, SyncPromoNotSignedIn);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBubbleGtkBrowserTest, SyncPromoLink);

  BookmarkBubbleGtk(GtkWidget* anchor,
                    Profile* profile,
                    const GURL& url,
                    bool newly_bookmarked);
  virtual ~BookmarkBubbleGtk();

  
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnDestroy);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnNameActivate);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnFolderChanged);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnEditClicked);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnCloseClicked);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnRemoveClicked);
  CHROMEGTK_CALLBACK_1(BookmarkBubbleGtk,
                       gboolean,
                       OnSignInClicked,
                       gchar*);
  CHROMEGTK_CALLBACK_0(BookmarkBubbleGtk, void, OnSyncPromoRealize);
  CHROMEGTK_CALLBACK_1(BookmarkBubbleGtk,
                       gboolean,
                       OnSyncPromoExpose,
                       GdkEventExpose*);

  
  void UpdatePromoColors();

  
  void ApplyEdits();

  
  void ShowEditor();

  
  std::string GetTitle();

  void InitFolderComboModel();

  
  
  static BookmarkBubbleGtk* bookmark_bubble_;

  
  GURL url_;

  
  Profile* profile_;

  
  BookmarkModel* model_;

  
  GtkThemeService* theme_service_;

  
  GtkWidget* anchor_;

  
  GtkWidget* remove_button_;

  
  GtkWidget* promo_;

  
  GtkWidget* promo_label_;

  
  
  std::vector<GtkWidget*> labels_;

  
  GtkWidget* name_entry_;

  
  GtkWidget* folder_combo_;
  scoped_ptr<RecentlyUsedFoldersComboModel> folder_combo_model_;

  BubbleGtk* bubble_;

  
  bool newly_bookmarked_;
  
  bool apply_edits_;
  bool remove_bookmark_;

  content::NotificationRegistrar registrar_;

  
  
  base::WeakPtrFactory<BookmarkBubbleGtk> factory_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBubbleGtk);
};

#endif  
