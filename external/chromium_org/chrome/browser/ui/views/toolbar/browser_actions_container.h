// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_ACTIONS_CONTAINER_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_ACTIONS_CONTAINER_H_

#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "chrome/browser/extensions/extension_toolbar_model.h"
#include "chrome/browser/ui/views/chrome_views_export.h"
#include "chrome/browser/ui/views/extensions/browser_action_overflow_menu_controller.h"
#include "chrome/browser/ui/views/extensions/extension_keybinding_registry_views.h"
#include "chrome/browser/ui/views/extensions/extension_popup.h"
#include "chrome/browser/ui/views/toolbar/browser_action_view.h"
#include "content/public/browser/notification_observer.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/tween.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/controls/resize_area_delegate.h"
#include "ui/views/drag_controller.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget_observer.h"

class BrowserActionButton;
class ExtensionKeybindingRegistryViews;
class ExtensionPopup;

namespace extensions {
class ActiveTabPermissionGranter;
}

namespace gfx {
class SlideAnimation;
}

namespace views {
class ResizeArea;
}

class BrowserActionsContainer
    : public views::View,
      public views::MenuButtonListener,
      public views::ResizeAreaDelegate,
      public gfx::AnimationDelegate,
      public ExtensionToolbarModel::Observer,
      public BrowserActionOverflowMenuController::Observer,
      public views::WidgetObserver,
      public BrowserActionView::Delegate,
      public extensions::ExtensionKeybindingRegistry::Delegate {
 public:
  BrowserActionsContainer(Browser* browser, views::View* owner_view);
  virtual ~BrowserActionsContainer();

  void Init();

  
  int num_browser_actions() const { return browser_action_views_.size(); }

  
  bool animating() const { return animation_target_size_ > 0; }

  
  views::View* chevron() { return chevron_; }
  const views::View* chevron() const { return chevron_; }

  
  Profile* profile() const { return profile_; }

  
  BrowserActionView* GetBrowserActionViewAt(int index) {
    return browser_action_views_[index];
  }

  
  BrowserActionView* GetBrowserActionView(ExtensionAction* action);

  
  void RefreshBrowserActionViews();

  
  void CreateBrowserActionViews();

  
  void DeleteBrowserActionViews();

  
  size_t VisibleBrowserActions() const;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool GetDropFormats(int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;

  
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
  virtual void OnResize(int resize_amount, bool done_resizing) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  virtual void NotifyMenuDeleted(
      BrowserActionOverflowMenuController* controller) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  virtual void InspectPopup(ExtensionAction* action) OVERRIDE;
  virtual int GetCurrentTabId() const OVERRIDE;
  virtual void OnBrowserActionExecuted(BrowserActionButton* button) OVERRIDE;
  virtual void OnBrowserActionVisibilityChanged() OVERRIDE;
  virtual gfx::Point GetViewContentOffset() const OVERRIDE;

  
  virtual extensions::ActiveTabPermissionGranter*
      GetActiveTabPermissionGranter() OVERRIDE;

  
  void MoveBrowserAction(const std::string& extension_id, size_t new_index);

  
  void HidePopup();

  
  
  void TestExecuteBrowserAction(int index);

  
  ExtensionPopup* TestGetPopup() { return popup_; }

  
  
  void TestSetIconVisibilityCount(size_t icons);

  
  
  
  static bool disable_animations_during_testing_;

 protected:
  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

 private:
  friend class BrowserActionView;  
  friend class ShowFolderMenuTask;

  typedef std::vector<BrowserActionView*> BrowserActionViews;

  
  static int IconWidth(bool include_padding);

  
  static int IconHeight();

  
  virtual void BrowserActionAdded(const extensions::Extension* extension,
                                  int index) OVERRIDE;
  virtual void BrowserActionRemoved(
      const extensions::Extension* extension) OVERRIDE;
  virtual void BrowserActionMoved(const extensions::Extension* extension,
                                  int index) OVERRIDE;
  virtual bool BrowserActionShowPopup(
      const extensions::Extension* extension) OVERRIDE;
  virtual void VisibleCountChanged() OVERRIDE;

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

  
  
  
  void SaveDesiredSizeAndAnimate(gfx::Tween::Type type,
                                 size_t num_visible_icons);

  
  
  
  bool ShouldDisplayBrowserAction(const extensions::Extension* extension);

  
  
  
  bool ShowPopup(BrowserActionButton* button,
                 ExtensionPopup::ShowAction show_action,
                 bool should_grant);

  
  
  
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

  
  scoped_ptr<gfx::SlideAnimation> resize_animation_;

  
  bool suppress_chevron_;

  
  
  
  int resize_amount_;

  
  
  int animation_target_size_;

  
  int drop_indicator_position_;

  
  scoped_ptr<ExtensionKeybindingRegistryViews> extension_keybinding_registry_;

  base::WeakPtrFactory<BrowserActionsContainer> task_factory_;

  
  base::WeakPtrFactory<BrowserActionsContainer> show_menu_task_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionsContainer);
};

#endif  
