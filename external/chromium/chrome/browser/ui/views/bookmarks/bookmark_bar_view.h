// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#pragma once

#include <set>

#include "base/compiler_specific.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_bar_instructions_view.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_menu_controller_views.h"
#include "chrome/browser/ui/views/detachable_toolbar_view.h"
#include "content/common/notification_registrar.h"
#include "ui/base/animation/animation_delegate.h"
#include "views/controls/button/button.h"
#include "views/controls/menu/view_menu_delegate.h"

class Browser;
class PageNavigator;
class PrefService;

namespace ui {
class SlideAnimation;
}

namespace views {
class CustomButton;
class MenuButton;
class MenuItemView;
class TextButton;
}

class BookmarkBarView : public DetachableToolbarView,
                        public ProfileSyncServiceObserver,
                        public BookmarkModelObserver,
                        public views::ViewMenuDelegate,
                        public views::ButtonListener,
                        public NotificationObserver,
                        public views::ContextMenuController,
                        public views::DragController,
                        public ui::AnimationDelegate,
                        public BookmarkMenuController::Observer,
                        public BookmarkBarInstructionsView::Delegate {
  friend class ShowFolderMenuTask;

 public:
  
  
  static const int kNewtabHorizontalPadding;
  static const int kNewtabVerticalPadding;

  
  static const int kMaxButtonWidth;

  
  
  
  
  
  
  
  
  
  
  
  
  class ModelChangedListener {
   public:
    virtual ~ModelChangedListener() {}

    
    
    virtual void ModelChanged() = 0;
  };

  static const int kNewtabBarHeight;

  BookmarkBarView(Profile* profile, Browser* browser);
  virtual ~BookmarkBarView();

  
  
  void SetProfile(Profile* profile);

  
  Profile* GetProfile() { return profile_; }

  
  Browser* browser() const { return browser_; }

  
  
  void SetPageNavigator(PageNavigator* navigator);

  
  
  void set_infobar_visible(bool infobar_visible) {
    infobar_visible_ = infobar_visible;
  }

  virtual bool IsOnTop() const;

  
  virtual bool IsDetached() const OVERRIDE;
  virtual double GetAnimationValue() const OVERRIDE;
  virtual int GetToolbarOverlap() const OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add, View* parent, View* child)
      OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const views::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const views::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const views::DropTargetEvent& event) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p, bool is_mouse_gesture)
      OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnStateChanged();

  
  void OnFullscreenToggled(bool fullscreen);

  
  void SetModelChangedListener(ModelChangedListener* listener) {
    model_changed_listener_ = listener;
  }

  
  
  void ClearModelChangedListenerIfEquals(ModelChangedListener* listener) {
    if (model_changed_listener_ == listener)
      model_changed_listener_ = NULL;
  }

  
  ModelChangedListener* GetModelChangedListener() {
    return model_changed_listener_;
  }

  
  PageNavigator* GetPageNavigator() { return page_navigator_; }

  
  BookmarkModel* GetModel() { return model_; }

  
  bool IsAlwaysShown() const;

  
  bool OnNewTabPage() const;

  
  
  int GetToolbarOverlap(bool return_max) const;

  
  bool is_animating();

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  virtual void BookmarkMenuDeleted(BookmarkMenuController* controller);

  
  views::TextButton* GetBookmarkButton(int index);

  
  
  views::MenuButton* other_bookmarked_button() const {
    return other_bookmarked_button_;
  }

  
  views::MenuItemView* GetMenu();

  
  views::MenuItemView* GetDropMenu();

  
  views::MenuItemView* GetContextMenu();

  
  views::MenuButton* overflow_button() const { return overflow_button_; }

  
  
  
  
  
  const BookmarkNode* GetNodeForButtonAt(const gfx::Point& loc,
                                         int* start_index);

  
  views::MenuButton* GetMenuButtonForNode(const BookmarkNode* node);

  
  
  void GetAnchorPositionAndStartIndexForButton(
      views::MenuButton* button,
      views::MenuItemView::AnchorPosition* anchor,
      int* start_index);

  
  virtual void ShowImportDialog();

  
  
  
  void StopThrobbing(bool immediate);

  
  
  
  int GetBookmarkButtonCount();

  
  
  
  
  
  static std::wstring CreateToolTipForURLAndTitle(
      const gfx::Point& screen_loc,
      const GURL& url,
      const std::wstring& title,
      Profile* profile);

  
  static bool testing_;

 private:
  class ButtonSeparatorView;
  struct DropInfo;

  
  
  class ShowFolderDropMenuTask : public Task {
   public:
    ShowFolderDropMenuTask(BookmarkBarView* view, const BookmarkNode* node)
      : view_(view),
        node_(node) {
    }

    void Cancel() {
      view_->show_folder_drop_menu_task_ = NULL;
      view_ = NULL;
    }

    virtual void Run() {
      if (view_) {
        view_->show_folder_drop_menu_task_ = NULL;
        view_->ShowDropFolderForNode(node_);
      }
      
    }

   private:
    BookmarkBarView* view_;
    const BookmarkNode* node_;

    DISALLOW_COPY_AND_ASSIGN(ShowFolderDropMenuTask);
  };

  
  
  void Init();

  
  views::MenuButton* CreateOtherBookmarkedButton();

  
  views::MenuButton* CreateOverflowButton();

  
  
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

  
  void BookmarkNodeAddedImpl(BookmarkModel* model,
                             const BookmarkNode* parent,
                             int index);

  
  
  
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node);

  
  void BookmarkNodeRemovedImpl(BookmarkModel* model,
                               const BookmarkNode* parent,
                               int index);

  
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node);

  
  
  void BookmarkNodeChangedImpl(BookmarkModel* model, const BookmarkNode* node);

  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node);

  
  
  
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node);

  
  
  virtual void WriteDragDataForView(views::View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) OVERRIDE;

  virtual int GetDragOperationsForView(views::View* sender,
                                       const gfx::Point& p) OVERRIDE;

  virtual bool CanStartDragForView(views::View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
  void WriteBookmarkDragData(const BookmarkNode* node,
                             ui::OSExchangeData* data);

  
  
  virtual void RunMenu(views::View* view, const gfx::Point& pt);

  
  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& p,
                                      bool is_mouse_gesture);

  
  views::View* CreateBookmarkButton(const BookmarkNode* node);

  
  
  void ConfigureButton(const BookmarkNode* node, views::TextButton* button);

  
  
  
  virtual bool IsItemChecked(int id) const;

  
  
  virtual void ExecuteCommand(int id);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnThemeChanged();

  
  void NotifyModelChanged();

  
  void ShowDropFolderForNode(const BookmarkNode* node);

  
  void StopShowFolderDropMenuTimer();

  
  void StartShowFolderDropMenuTimer(const BookmarkNode* node);

  
  
  int CalculateDropOperation(const views::DropTargetEvent& event,
                             const BookmarkNodeData& data,
                             int* index,
                             bool* drop_on,
                             bool* is_over_overflow,
                             bool* is_over_other);

  
  
  int GetFirstHiddenNodeIndex();

  
  
  
  
  void StartThrobbing(const BookmarkNode* node, bool overflow_only);

  
  
  
  views::CustomButton* DetermineViewToThrobFromRemove(
      const BookmarkNode* parent,
      int old_index);

  
  void UpdateColors();

  
  
  void UpdateOtherBookmarksVisibility();

  
  
  
  
  gfx::Size LayoutItems(bool compute_bounds_only);

  
  views::TextButton* CreateSyncErrorButton();

  NotificationRegistrar registrar_;

  Profile* profile_;

  
  PageNavigator* page_navigator_;

  
  
  BookmarkModel* model_;

  
  
  BookmarkMenuController* bookmark_menu_;

  
  
  
  BookmarkMenuController* bookmark_drop_menu_;

  
  views::MenuButton* other_bookmarked_button_;

  
  ModelChangedListener* model_changed_listener_;

  
  ShowFolderDropMenuTask* show_folder_drop_menu_task_;

  
  scoped_ptr<DropInfo> drop_info_;

  
  
  views::TextButton* sync_error_button_;

  
  ProfileSyncService* sync_service_;

  
  views::MenuButton* overflow_button_;

  
  
  views::View* instructions_;

  ButtonSeparatorView* bookmarks_separator_view_;

  
  Browser* browser_;

  
  bool infobar_visible_;

  
  scoped_ptr<ui::SlideAnimation> size_animation_;

  
  
  
  views::CustomButton* throbbing_view_;

  
  SkBitmap toolstrip_background_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBarView);
};

#endif  
