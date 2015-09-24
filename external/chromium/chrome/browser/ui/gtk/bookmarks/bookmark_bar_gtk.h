// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BAR_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/bookmark_context_menu_controller.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/ui/gtk/bookmarks/bookmark_bar_instructions_gtk.h"
#include "chrome/browser/ui/gtk/menu_bar_helper.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/gtk/view_id_util.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/animation/animation.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

class BookmarkMenuController;
class Browser;
class BrowserWindowGtk;
class CustomContainerButton;
class GtkThemeService;
class MenuGtk;
class PageNavigator;
class Profile;
class TabstripOriginProvider;

class BookmarkBarGtk : public ui::AnimationDelegate,
                       public ProfileSyncServiceObserver,
                       public BookmarkModelObserver,
                       public MenuBarHelper::Delegate,
                       public NotificationObserver,
                       public BookmarkBarInstructionsGtk::Delegate,
                       public BookmarkContextMenuControllerDelegate {
 public:
  BookmarkBarGtk(BrowserWindowGtk* window,
                 Profile* profile,
                 Browser* browser,
                 TabstripOriginProvider* tabstrip_origin_provider);
  virtual ~BookmarkBarGtk();

  
  
  void SetProfile(Profile* profile);

  
  Profile* GetProfile() { return profile_; }

  
  Browser* browser() const { return browser_; }

  
  GtkWidget* widget() const { return event_box_.get(); }

  
  
  void SetPageNavigator(PageNavigator* navigator);

  
  void Init(Profile* profile);

  
  
  bool OnNewTabPage();

  
  
  
  
  
  
  
  
  void Show(bool animate);
  void Hide(bool animate);
  void EnterFullscreen();

  
  int GetHeight();

  
  bool IsAnimating();

  
  bool IsAlwaysShown();

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  virtual void PopupForButton(GtkWidget* button);
  virtual void PopupForButtonNextTo(GtkWidget* button,
                                    GtkMenuDirectionType dir);

  
  static const int kBookmarkBarNTPHeight;

  
  virtual void CloseMenu();

  const ui::Animation* animation() { return &slide_animation_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarGtkUnittest, DisplaysHelpMessageOnEmpty);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarGtkUnittest,
                           HidesHelpMessageWithBookmark);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarGtkUnittest, BuildsButtons);

  
  void CreateAllBookmarkButtons();

  
  
  void SetInstructionState();

  
  void SetChevronState();

  
  void RemoveAllBookmarkButtons();

  
  
  
  int GetBookmarkButtonCount();

  
  
  void SetOverflowButtonAppearance();

  
  
  
  
  
  
  
  int GetFirstHiddenBookmark(int extra_space,
                             std::vector<GtkWidget*>* showing_folders);

  
  
  bool ShouldBeFloating();
  
  void UpdateFloatingState();

  
  
  void UpdateEventBoxPaintability();

  
  void PaintEventBox();

  
  
  
  bool GetTabContentsSize(gfx::Size* size);

  
  
  
  
  void StartThrobbingAfterAllocation(GtkWidget* item);

  
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnItemAllocate, GtkAllocation*);

  
  
  void StartThrobbing(const BookmarkNode* node);

  
  
  void SetThrobbingWidget(GtkWidget* widget);

  
  
  gboolean ItemDraggedOverToolbar(
      GdkDragContext* context, int index, guint time);

  
  
  
  
  
  int GetToolbarIndexForDragOverFolder(GtkWidget* button, gint x);

  void ClearToolbarDropHighlighting();

  

  
  
  virtual void Loaded(BookmarkModel* model);

  
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model);

  
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
                                   const BookmarkNode* node);
  
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node);
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  GtkWidget* CreateBookmarkButton(const BookmarkNode* node);
  GtkToolItem* CreateBookmarkToolItem(const BookmarkNode* node);

  void ConnectFolderButtonEvents(GtkWidget* widget, bool is_tool_item);

  
  const BookmarkNode* GetNodeForToolButton(GtkWidget* button);

  
  void PopupMenuForNode(GtkWidget* sender, const BookmarkNode* node,
                        GdkEventButton* event);

  
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, gboolean, OnButtonPressed,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, gboolean, OnSyncErrorButtonPressed,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnClicked);
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnButtonDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnButtonDragEnd, GdkDragContext*);
  CHROMEGTK_CALLBACK_4(BookmarkBarGtk, void, OnButtonDragGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);

  
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnFolderClicked);

  
  CHROMEGTK_CALLBACK_4(BookmarkBarGtk, gboolean, OnToolbarDragMotion,
                       GdkDragContext*, gint, gint, guint);
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnToolbarSizeAllocate,
                       GtkAllocation*);

  
  CHROMEGTK_CALLBACK_6(BookmarkBarGtk, void, OnDragReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);
  CHROMEGTK_CALLBACK_2(BookmarkBarGtk, void, OnDragLeave,
                       GdkDragContext*, guint);

  
  CHROMEGTK_CALLBACK_4(BookmarkBarGtk, gboolean, OnFolderDragMotion,
                       GdkDragContext*, gint, gint, guint);

  
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, gboolean, OnEventBoxExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnEventBoxDestroy);

  
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnParentSizeAllocate,
                       GtkAllocation*);

  
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnThrobbingWidgetDestroy);

  
  virtual void OnStateChanged();

  
  virtual void ShowImportDialog();

  
  void OnEditBookmarksEnabledChanged();

  Profile* profile_;

  
  PageNavigator* page_navigator_;

  Browser* browser_;
  BrowserWindowGtk* window_;

  
  TabstripOriginProvider* tabstrip_origin_provider_;

  
  
  BookmarkModel* model_;

  
  
  OwnedWidgetGtk event_box_;

  
  GtkWidget* ntp_padding_box_;

  
  GtkWidget* paint_box_;

  
  GtkWidget* bookmark_hbox_;

  
  
  GtkWidget* instructions_;

  
  
  scoped_ptr<BookmarkBarInstructionsGtk> instructions_gtk_;

  
  OwnedWidgetGtk bookmark_toolbar_;

  
  
  GtkWidget* overflow_button_;

  
  GtkWidget* other_bookmarks_button_;

  
  GtkWidget* sync_error_button_;

  
  ProfileSyncService* sync_service_;

  
  
  const BookmarkNode* dragged_node_;

  
  GtkWidget* drag_icon_;

  
  GtkToolItem* toolbar_drop_item_;

  
  GtkThemeService* theme_service_;

  
  bool show_instructions_;

  MenuBarHelper menu_bar_helper_;

  
  
  
  scoped_ptr<BookmarkContextMenuController> current_context_menu_controller_;
  
  scoped_ptr<MenuGtk> current_context_menu_;

  
  
  scoped_ptr<BookmarkMenuController> current_menu_;

  ui::SlideAnimation slide_animation_;

  
  
  
  bool floating_;

  
  
  int last_allocation_width_;

  NotificationRegistrar registrar_;

  
  
  gfx::Size last_tab_contents_size_;

  
  
  gfx::Point last_pressed_coordinates_;

  
  
  GtkWidget* throbbing_widget_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  ScopedRunnableMethodFactory<BookmarkBarGtk> method_factory_;
};

#endif  
