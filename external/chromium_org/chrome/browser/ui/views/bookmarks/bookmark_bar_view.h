// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "chrome/browser/bookmarks/bookmark_stats.h"
#include "chrome/browser/ui/bookmarks/bookmark_bar.h"
#include "chrome/browser/ui/bookmarks/bookmark_bar_instructions_delegate.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_bubble_view_observer.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_menu_controller_observer.h"
#include "chrome/browser/ui/views/detachable_toolbar_view.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/drag_controller.h"

class BookmarkContextMenu;
class Browser;
class BrowserView;

namespace content {
class PageNavigator;
}

namespace gfx {
class SlideAnimation;
}

namespace views {
class CustomButton;
class MenuButton;
class TextButton;
}

class BookmarkBarView : public DetachableToolbarView,
                        public BookmarkModelObserver,
                        public views::MenuButtonListener,
                        public views::ButtonListener,
                        public views::ContextMenuController,
                        public views::DragController,
                        public gfx::AnimationDelegate,
                        public BookmarkMenuControllerObserver,
                        public BookmarkBarInstructionsDelegate,
                        public BookmarkBubbleViewObserver {
 public:
  
  static const char kViewClassName[];

  
  
  static const int kNewtabHorizontalPadding;

  
  static const int kMaxButtonWidth;

  
  static const int kToolbarAttachedBookmarkBarOverlap;

  
  BookmarkBarView(Browser* browser, BrowserView* browser_view);
  virtual ~BookmarkBarView();

  static void DisableAnimationsForTesting(bool disabled);

  
  Browser* browser() const { return browser_; }

  
  
  void SetPageNavigator(content::PageNavigator* navigator);

  
  
  void set_infobar_visible(bool infobar_visible) {
    infobar_visible_ = infobar_visible;
  }

  
  void SetBookmarkBarState(BookmarkBar::State state,
                           BookmarkBar::AnimateChangeType animate_type);

  
  int GetFullyDetachedToolbarOverlap() const;

  
  bool is_animating();

  
  
  
  
  
  const BookmarkNode* GetNodeForButtonAtModelIndex(const gfx::Point& loc,
                                                   int* model_start_index);

  
  views::MenuButton* GetMenuButtonForNode(const BookmarkNode* node);

  
  void GetAnchorPositionForButton(views::MenuButton* button,
                                  views::MenuItemView::AnchorPosition* anchor);

  
  
  views::MenuButton* other_bookmarked_button() const {
    return other_bookmarked_button_;
  }

  
  views::MenuButton* overflow_button() const { return overflow_button_; }

  
  views::MenuItemView* GetMenu();

  
  views::MenuItemView* GetContextMenu();

  
  views::MenuItemView* GetDropMenu();

  
  
  
  void StopThrobbing(bool immediate);

  
  
  
  
  
  
  static base::string16 CreateToolTipForURLAndTitle(const views::Widget* widget,
                                              const gfx::Point& screen_loc,
                                              const GURL& url,
                                              const base::string16& title,
                                              Profile* profile);

  
  virtual bool IsDetached() const OVERRIDE;
  virtual double GetAnimationValue() const OVERRIDE;
  virtual int GetToolbarOverlap() const OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  virtual void BookmarkMenuControllerDeleted(
      BookmarkMenuController* controller) OVERRIDE;

  
  virtual void ShowImportDialog() OVERRIDE;

  
  virtual void OnBookmarkBubbleShown(const GURL& url) OVERRIDE;
  virtual void OnBookmarkBubbleHidden() OVERRIDE;

  
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
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;

  
  virtual void WriteDragDataForView(views::View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(views::View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(views::View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* view,
                                   const gfx::Point& point) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

 private:
  class ButtonSeparatorView;
  struct DropInfo;
  struct DropLocation;

  friend class BookmarkBarViewEventTestBase;
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarViewTest, SwitchProfile);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarViewTest,
                           NoAppsShortcutWithoutInstantExtended);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBarViewInstantExtendedTest,
                           AppsShortcutVisibility);

  
  enum DropButtonType {
    DROP_BOOKMARK,
    DROP_OTHER_FOLDER,
    DROP_OVERFLOW
  };

  
  
  void Init();

  
  
  
  
  
  

  
  
  
  int GetBookmarkButtonCount();

  
  views::TextButton* GetBookmarkButton(int index);

  
  
  BookmarkLaunchLocation GetBookmarkLaunchLocation() const;

  
  
  int GetFirstHiddenNodeIndex();

  
  views::MenuButton* CreateOtherBookmarkedButton();

  
  views::MenuButton* CreateOverflowButton();

  
  views::View* CreateBookmarkButton(const BookmarkNode* node);

  
  views::TextButton* CreateAppsPageShortcutButton();

  
  
  void ConfigureButton(const BookmarkNode* node, views::TextButton* button);

  
  void BookmarkNodeAddedImpl(BookmarkModel* model,
                             const BookmarkNode* parent,
                             int index);

  
  void BookmarkNodeRemovedImpl(BookmarkModel* model,
                               const BookmarkNode* parent,
                               int index);

  
  
  void BookmarkNodeChangedImpl(BookmarkModel* model, const BookmarkNode* node);

  
  void ShowDropFolderForNode(const BookmarkNode* node);

  
  void StopShowFolderDropMenuTimer();

  
  void StartShowFolderDropMenuTimer(const BookmarkNode* node);

  
  void CalculateDropLocation(const ui::DropTargetEvent& event,
                             const BookmarkNodeData& data,
                             DropLocation* location);

  
  void WriteBookmarkDragData(const BookmarkNode* node,
                             ui::OSExchangeData* data);

  
  
  
  
  void StartThrobbing(const BookmarkNode* node, bool overflow_only);

  
  
  
  views::CustomButton* DetermineViewToThrobFromRemove(
      const BookmarkNode* parent,
      int old_index);

  
  void UpdateColors();

  
  
  void UpdateOtherBookmarksVisibility();

  
  void UpdateBookmarksSeparatorVisibility();

  
  
  
  
  gfx::Size LayoutItems(bool compute_bounds_only);

  
  void OnAppsPageShortcutVisibilityPrefChanged();

  
  PrefChangeRegistrar profile_pref_registrar_;

  
  content::PageNavigator* page_navigator_;

  
  
  BookmarkModel* model_;

  
  
  BookmarkMenuController* bookmark_menu_;

  
  
  
  BookmarkMenuController* bookmark_drop_menu_;

  
  
  scoped_ptr<BookmarkContextMenu> context_menu_;

  
  views::MenuButton* other_bookmarked_button_;

  
  views::TextButton* apps_page_shortcut_;

  
  base::WeakPtrFactory<BookmarkBarView> show_folder_method_factory_;

  
  scoped_ptr<DropInfo> drop_info_;

  
  views::MenuButton* overflow_button_;

  
  
  views::View* instructions_;

  ButtonSeparatorView* bookmarks_separator_view_;

  Browser* browser_;
  BrowserView* browser_view_;

  
  bool infobar_visible_;

  
  scoped_ptr<gfx::SlideAnimation> size_animation_;

  
  
  
  views::CustomButton* throbbing_view_;

  BookmarkBar::State bookmark_bar_state_;

  
  bool animating_detached_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBarView);
};

#endif  
