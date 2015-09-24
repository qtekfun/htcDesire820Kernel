// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BROWSER_ACTIONS_CONTAINER_H_
#define CHROME_BROWSER_UI_VIEWS_BROWSER_ACTIONS_CONTAINER_H_
#pragma once

#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/task.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/extensions/extension_toolbar_model.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/ui/views/browser_bubble.h"
#include "chrome/browser/ui/views/extensions/browser_action_overflow_menu_controller.h"
#include "chrome/browser/ui/views/extensions/extension_popup.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/tween.h"
#include "views/controls/button/menu_button.h"
#include "views/controls/menu/view_menu_delegate.h"
#include "views/controls/resize_area.h"
#include "views/view.h"

class Browser;
class BrowserActionOverflowMenuController;
class BrowserActionsContainer;
class Extension;
class ExtensionAction;
class ExtensionPopup;
class PrefService;
class Profile;

namespace gfx {
class CanvasSkia;
}

namespace ui {
class SlideAnimation;
}

namespace views {
class Menu2;
}


class BrowserActionButton : public views::MenuButton,
                            public views::ButtonListener,
                            public ImageLoadingTracker::Observer,
                            public NotificationObserver {
 public:
  BrowserActionButton(const Extension* extension,
                      BrowserActionsContainer* panel);

  
  void Destroy();

  ExtensionAction* browser_action() const { return browser_action_; }
  const Extension* extension() { return extension_; }

  
  void UpdateState();

  
  const SkBitmap& default_icon() const { return default_icon_; }

  
  virtual bool IsPopup();
  virtual GURL GetPopupUrl();

  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index) OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  
  
  
  
  virtual bool Activate() OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const views::KeyEvent& event) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p,
                               bool is_mouse_gesture) OVERRIDE;

  
  
  void SetButtonPushed();
  void SetButtonNotPushed();

 protected:
  
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;

 private:
  virtual ~BrowserActionButton();

  
  
  ExtensionAction* browser_action_;

  
  const Extension* extension_;

  
  
  ImageLoadingTracker tracker_;

  
  bool showing_context_menu_;

  
  
  SkBitmap default_icon_;

  
  BrowserActionsContainer* panel_;

  scoped_refptr<ExtensionContextMenuModel> context_menu_contents_;
  scoped_ptr<views::Menu2> context_menu_menu_;

  NotificationRegistrar registrar_;

  friend class DeleteTask<BrowserActionButton>;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionButton);
};



class BrowserActionView : public views::View {
 public:
  BrowserActionView(const Extension* extension, BrowserActionsContainer* panel);
  virtual ~BrowserActionView();

  BrowserActionButton* button() { return button_; }

  
  
  
  gfx::Canvas* GetIconWithBadge();

  
  virtual void Layout() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

 private:
  
  BrowserActionsContainer* panel_;

  
  BrowserActionButton* button_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionView);
};

class BrowserActionsContainer
    : public views::View,
      public views::ViewMenuDelegate,
      public views::DragController,
      public views::ResizeArea::ResizeAreaDelegate,
      public ui::AnimationDelegate,
      public ExtensionToolbarModel::Observer,
      public BrowserActionOverflowMenuController::Observer,
      public ExtensionContextMenuModel::PopupDelegate,
      public ExtensionPopup::Observer {
 public:
  BrowserActionsContainer(Browser* browser, views::View* owner_view);
  virtual ~BrowserActionsContainer();

  static void RegisterUserPrefs(PrefService* prefs);

  void Init();

  
  int num_browser_actions() const { return browser_action_views_.size(); }

  
  bool animating() const { return animation_target_size_ > 0; }

  
  const views::View* chevron() const { return chevron_; }

  
  Profile* profile() const { return profile_; }

  
  Browser* browser() const { return browser_; }

  
  int GetCurrentTabId() const;

  
  BrowserActionView* GetBrowserActionViewAt(int index) {
    return browser_action_views_[index];
  }

  
  BrowserActionView* GetBrowserActionView(ExtensionAction* action);

  
  void RefreshBrowserActionViews();

  
  void CreateBrowserActionViews();

  
  void DeleteBrowserActionViews();

  
  void OnBrowserActionVisibilityChanged();

  
  size_t VisibleBrowserActions() const;

  
  void OnBrowserActionExecuted(BrowserActionButton* button,
                               bool inspect_with_devtools);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool GetDropFormats(int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const views::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const views::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const views::DropTargetEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void RunMenu(View* source, const gfx::Point& pt) OVERRIDE;

  
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
  virtual void OnResize(int resize_amount, bool done_resizing) OVERRIDE;

  
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const ui::Animation* animation) OVERRIDE;

  
  virtual void NotifyMenuDeleted(
      BrowserActionOverflowMenuController* controller) OVERRIDE;

  
  virtual void InspectPopup(ExtensionAction* action) OVERRIDE;

  
  virtual void ExtensionPopupIsClosing(ExtensionPopup* popup) OVERRIDE;

  
  void MoveBrowserAction(const std::string& extension_id, size_t new_index);

  
  void HidePopup();

  
  
  void TestExecuteBrowserAction(int index);

  
  ExtensionPopup* TestGetPopup() { return popup_; }

  
  
  void TestSetIconVisibilityCount(size_t icons);

  
  
  
  static bool disable_animations_during_testing_;

 protected:
  
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

 private:
  friend class BrowserActionView;  
  friend class ShowFolderMenuTask;

  typedef std::vector<BrowserActionView*> BrowserActionViews;

  
  static int IconWidth(bool include_padding);

  
  static int IconHeight();

  
  virtual void BrowserActionAdded(const Extension* extension, int index);
  virtual void BrowserActionRemoved(const Extension* extension);
  virtual void BrowserActionMoved(const Extension* extension, int index);
  virtual void ModelLoaded();

  void LoadImages();

  
  void SetContainerWidth();

  
  void CloseOverflowMenu();

  
  void StopShowFolderDropMenuTimer();

  
  void StartShowFolderDropMenuTimer();

  
  void ShowDropFolder();

  
  
  void SetDropIndicator(int x_pos);

  
  
  
  int IconCountToWidth(int icons, bool display_chevron) const;

  
  
  
  size_t WidthToIconCount(int pixels) const;

  
  
  
  
  int ContainerMinSize() const;

  
  
  
  void SaveDesiredSizeAndAnimate(ui::Tween::Type type,
                                 size_t num_visible_icons);

  
  
  
  bool ShouldDisplayBrowserAction(const Extension* extension);

  
  
  
  BrowserActionViews browser_action_views_;

  Profile* profile_;

  
  Browser* browser_;

  
  views::View* owner_view_;

  
  ExtensionPopup* popup_;

  
  
  BrowserActionButton* popup_button_;

  
  ExtensionToolbarModel* model_;

  
  int container_width_;

  
  views::ResizeArea* resize_area_;

  
  views::MenuButton* chevron_;

  
  
  BrowserActionOverflowMenuController* overflow_menu_;

  
  scoped_ptr<ui::SlideAnimation> resize_animation_;

  
  bool suppress_chevron_;

  
  
  
  int resize_amount_;

  
  
  int animation_target_size_;

  
  int drop_indicator_position_;

  ScopedRunnableMethodFactory<BrowserActionsContainer> task_factory_;

  
  ScopedRunnableMethodFactory<BrowserActionsContainer> show_menu_task_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionsContainer);
};

#endif  
