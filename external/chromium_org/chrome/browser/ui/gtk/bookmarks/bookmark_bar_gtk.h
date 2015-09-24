// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_BAR_GTK_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_stats.h"
#include "chrome/browser/ui/bookmarks/bookmark_bar.h"
#include "chrome/browser/ui/bookmarks/bookmark_bar_instructions_delegate.h"
#include "chrome/browser/ui/bookmarks/bookmark_context_menu_controller.h"
#include "chrome/browser/ui/gtk/menu_bar_helper.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

class BookmarkBarInstructionsGtk;
class BookmarkMenuController;
class Browser;
class BrowserWindowGtk;
class GtkThemeService;
class MenuGtk;
class TabstripOriginProvider;

namespace content {
class PageNavigator;
}

class BookmarkBarGtk : public gfx::AnimationDelegate,
                       public BookmarkModelObserver,
                       public MenuBarHelper::Delegate,
                       public content::NotificationObserver,
                       public BookmarkBarInstructionsDelegate,
                       public BookmarkContextMenuControllerDelegate {
 public:
  BookmarkBarGtk(BrowserWindowGtk* window,
                 Browser* browser,
                 TabstripOriginProvider* tabstrip_origin_provider);
  virtual ~BookmarkBarGtk();

  
  Browser* browser() const { return browser_; }

  
  GtkWidget* widget() const { return event_box_.get(); }

  
  
  void SetPageNavigator(content::PageNavigator* navigator);

  
  void Init();

  
  void SetBookmarkBarState(BookmarkBar::State state,
                           BookmarkBar::AnimateChangeType animate_type);

  
  int GetHeight();

  
  bool IsAnimating();

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  virtual void PopupForButton(GtkWidget* button) OVERRIDE;
  virtual void PopupForButtonNextTo(GtkWidget* button,
                                    GtkMenuDirectionType dir) OVERRIDE;

  
  virtual void CloseMenu() OVERRIDE;

  const gfx::Animation* animation() { return &slide_animation_; }

  int max_height() const { return max_height_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarGtkUnittest, DisplaysHelpMessageOnEmpty);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarGtkUnittest,
                           HidesHelpMessageWithBookmark);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarGtkUnittest, BuildsButtons);

  
  
  
  
  
  
  
  
  void Show(BookmarkBar::State old_state,
            BookmarkBar::AnimateChangeType animate_type);
  void Hide(BookmarkBar::State old_state,
            BookmarkBar::AnimateChangeType animate_type);

  
  void CalculateMaxHeight();

  
  void CreateAllBookmarkButtons();

  
  
  void SetInstructionState();

  
  void SetChevronState();

  
  
  void UpdateOtherBookmarksVisibility();

  
  void RemoveAllButtons();

  
  void AddCoreButtons();

  
  void ResetButtons();

  
  
  
  int GetBookmarkButtonCount();

  
  
  BookmarkLaunchLocation GetBookmarkLaunchLocation() const;

  
  
  void SetOverflowButtonAppearance();

  
  
  
  
  
  
  
  int GetFirstHiddenBookmark(int extra_space,
                             std::vector<GtkWidget*>* showing_folders);

  
  void UpdateDetachedState(BookmarkBar::State old_state);

  
  
  void UpdateEventBoxPaintability();

  
  void PaintEventBox();

  
  
  
  bool GetWebContentsSize(gfx::Size* size);

  
  
  
  
  void StartThrobbingAfterAllocation(GtkWidget* item);

  
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnItemAllocate, GtkAllocation*);

  
  
  void StartThrobbing(const BookmarkNode* node);

  
  
  void SetThrobbingWidget(GtkWidget* widget);

  
  
  gboolean ItemDraggedOverToolbar(
      GdkDragContext* context, int index, guint time);

  
  
  
  
  
  int GetToolbarIndexForDragOverFolder(GtkWidget* button, gint x);

  void ClearToolbarDropHighlighting();

  

  
  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;

  
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;

  
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) OVERRIDE;
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;
  
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  GtkWidget* CreateBookmarkButton(const BookmarkNode* node);
  GtkToolItem* CreateBookmarkToolItem(const BookmarkNode* node);

  void ConnectFolderButtonEvents(GtkWidget* widget, bool is_tool_item);

  
  const BookmarkNode* GetNodeForToolButton(GtkWidget* button);

  
  void PopupMenuForNode(GtkWidget* sender, const BookmarkNode* node,
                        GdkEventButton* event);

  
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, gboolean, OnButtonPressed,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnClicked);
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnButtonDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(BookmarkBarGtk, void, OnButtonDragEnd, GdkDragContext*);
  CHROMEGTK_CALLBACK_4(BookmarkBarGtk, void, OnButtonDragGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);

  
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnFolderClicked);

  
  CHROMEGTK_CALLBACK_0(BookmarkBarGtk, void, OnAppsButtonClicked);

  
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

  
  virtual void ShowImportDialog() OVERRIDE;

  
  
  void OnAppsPageShortcutVisibilityChanged();

  
  void OnEditBookmarksEnabledChanged();

  
  content::PageNavigator* page_navigator_;

  Browser* browser_;
  BrowserWindowGtk* window_;

  
  TabstripOriginProvider* tabstrip_origin_provider_;

  
  
  BookmarkModel* model_;

  
  
  ui::OwnedWidgetGtk event_box_;

  
  GtkWidget* ntp_padding_box_;

  
  GtkWidget* paint_box_;

  
  GtkWidget* bookmark_hbox_;

  
  
  GtkWidget* instructions_;

  
  
  scoped_ptr<BookmarkBarInstructionsGtk> instructions_gtk_;

  
  GtkWidget* apps_shortcut_button_;

  
  ui::OwnedWidgetGtk bookmark_toolbar_;

  
  
  GtkWidget* overflow_button_;

  
  
  GtkWidget* other_bookmarks_separator_;

  
  GtkWidget* other_bookmarks_button_;

  
  GtkWidget* other_padding_;

  
  
  const BookmarkNode* dragged_node_;

  
  GtkWidget* drag_icon_;

  
  GtkToolItem* toolbar_drop_item_;

  
  GtkThemeService* theme_service_;

  
  bool show_instructions_;

  MenuBarHelper menu_bar_helper_;

  
  
  
  scoped_ptr<BookmarkContextMenuController> current_context_menu_controller_;
  
  scoped_ptr<MenuGtk> current_context_menu_;

  
  
  scoped_ptr<BookmarkMenuController> current_menu_;

  gfx::SlideAnimation slide_animation_;

  
  
  int last_allocation_width_;

  content::NotificationRegistrar registrar_;

  
  
  gfx::Size last_web_contents_size_;

  
  
  gfx::Point last_pressed_coordinates_;

  
  
  GtkWidget* throbbing_widget_;

  
  BooleanPrefMember apps_shortcut_visible_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  BookmarkBar::State bookmark_bar_state_;

  
  int max_height_;

  base::WeakPtrFactory<BookmarkBarGtk> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBarGtk);
};

#endif  
