// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_VIEW_H_
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/command_updater.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/toolbar/back_forward_menu_model.h"
#include "chrome/browser/ui/views/accessible_pane_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/browser/ui/views/reload_button.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/models/accelerator.h"
#include "views/controls/button/menu_button.h"
#include "views/controls/menu/menu.h"
#include "views/controls/menu/menu_wrapper.h"
#include "views/controls/menu/view_menu_delegate.h"
#include "views/view.h"

class BrowserActionsContainer;
class Browser;
class Profile;
#if defined(OS_CHROMEOS)
namespace views {
class Menu2;
}  
#endif
class WrenchMenu;

class ToolbarView : public AccessiblePaneView,
                    public views::ViewMenuDelegate,
                    public ui::AcceleratorProvider,
                    public LocationBarView::Delegate,
                    public NotificationObserver,
                    public CommandUpdater::CommandObserver,
                    public views::ButtonListener {
 public:
  explicit ToolbarView(Browser* browser);
  virtual ~ToolbarView();

  
  void Init(Profile* profile);

  
  void SetProfile(Profile* profile);
  Profile* profile() { return profile_; }

  
  
  
  
  void Update(TabContents* tab, bool should_restore_state);

  
  
  
  void SetPaneFocusAndFocusLocationBar(int view_storage_id);

  
  
  
  void SetPaneFocusAndFocusAppMenu(int view_storage_id);

  
  bool IsAppMenuFocused();

  
  void AddMenuListener(views::MenuListener* listener);

  
  void RemoveMenuListener(views::MenuListener* listener);

  virtual bool GetAcceleratorInfo(int id, ui::Accelerator* accel);

  
  Browser* browser() const { return browser_; }
  BrowserActionsContainer* browser_actions() const { return browser_actions_; }
  ReloadButton* reload_button() const { return reload_; }
  LocationBarView* location_bar() const { return location_bar_; }
  views::MenuButton* app_menu() const { return app_menu_; }

  
  virtual bool SetPaneFocus(int view_storage_id, View* initial_focus) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void RunMenu(views::View* source, const gfx::Point& pt) OVERRIDE;

  
  virtual TabContentsWrapper* GetTabContentsWrapper() const OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual void OnInputInProgress(bool in_progress) OVERRIDE;

  
  virtual void EnabledStateChangedForCommand(int id, bool enabled) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event)
      OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual bool GetAcceleratorForCommandId(
      int command_id, ui::Accelerator* accelerator) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual int OnDragUpdated(const views::DropTargetEvent& event) OVERRIDE;
  virtual int OnPerformDrop(const views::DropTargetEvent& event) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

  
  
  static const int kStandardSpacing;
  
  
  static const int kVertSpacing;

 protected:

  
  virtual views::View* GetDefaultFocusableChild() OVERRIDE;
  virtual void RemovePaneFocus() OVERRIDE;

 private:
  
  bool IsUpgradeRecommended();

  
  int GetUpgradeRecommendedBadge() const;

  
  bool ShouldShowBackgroundPageBadge();

  
  bool ShouldShowIncompatibilityWarning();

  
  int PopupTopSpacing() const;

  
  void LoadImages();

  
  enum DisplayMode {
    DISPLAYMODE_NORMAL,       
    DISPLAYMODE_LOCATION      
                              
  };
  bool IsDisplayModeNormal() const {
    return display_mode_ == DISPLAYMODE_NORMAL;
  }

  
  void UpdateAppMenuBadge();

  
  
  SkBitmap GetAppMenuIcon(views::CustomButton::ButtonState state);

  
  
  SkBitmap GetBackgroundPageBadge();

  scoped_ptr<BackForwardMenuModel> back_menu_model_;
  scoped_ptr<BackForwardMenuModel> forward_menu_model_;

  
  ToolbarModel* model_;

  
  views::ImageButton* back_;
  views::ImageButton* forward_;
  ReloadButton* reload_;
  views::ImageButton* home_;
  LocationBarView* location_bar_;
  BrowserActionsContainer* browser_actions_;
  views::MenuButton* app_menu_;
  Profile* profile_;
  Browser* browser_;

  
  scoped_ptr<ui::SimpleMenuModel> profiles_menu_contents_;

  
  BooleanPrefMember show_home_button_;

  
  DisplayMode display_mode_;

  
  scoped_ptr<ui::SimpleMenuModel> wrench_menu_model_;

#if defined(OS_CHROMEOS)
  
  
  scoped_ptr<views::Menu2> wrench_menu_2_;
#endif

  
  scoped_refptr<WrenchMenu> wrench_menu_;

  
  std::vector<views::MenuListener*> menu_listeners_;

  NotificationRegistrar registrar_;

  
  
  
  bool* destroyed_flag_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ToolbarView);
};

#endif  
