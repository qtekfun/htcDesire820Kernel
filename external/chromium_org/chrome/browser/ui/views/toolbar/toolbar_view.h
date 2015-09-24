// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_VIEW_H_

#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/command_observer.h"
#include "chrome/browser/ui/toolbar/back_forward_menu_model.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/views/accessible_pane_view.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/view.h"

class BackButton;
class BrowserActionsContainer;
class Browser;
class HomeButton;
class ReloadButton;
class SiteChipView;
class ToolbarButton;
class WrenchMenu;
class WrenchMenuModel;
class WrenchToolbarButton;

namespace views {
class MenuListener;
}

class ToolbarView : public views::AccessiblePaneView,
                    public views::MenuButtonListener,
                    public ui::AcceleratorProvider,
                    public LocationBarView::Delegate,
                    public content::NotificationObserver,
                    public CommandObserver,
                    public views::ButtonListener,
                    public views::WidgetObserver {
 public:
  
  static const char kViewClassName[];

  explicit ToolbarView(Browser* browser);
  virtual ~ToolbarView();

  
  void Init();

  
  
  
  
  void Update(content::WebContents* tab);

  
  
  
  void SetPaneFocusAndFocusAppMenu();

  
  bool IsAppMenuFocused();

  
  void AddMenuListener(views::MenuListener* listener);

  
  void RemoveMenuListener(views::MenuListener* listener);

  virtual bool GetAcceleratorInfo(int id, ui::Accelerator* accel);

  
  views::View* GetBookmarkBubbleAnchor();

  
  views::View* GetTranslateBubbleAnchor();

  
  Browser* browser() const { return browser_; }
  BrowserActionsContainer* browser_actions() const { return browser_actions_; }
  ReloadButton* reload_button() const { return reload_; }
  LocationBarView* location_bar() const { return location_bar_; }
  SiteChipView* site_chip() const { return site_chip_view_; }
  views::MenuButton* app_menu() const;

  
  virtual bool SetPaneFocus(View* initial_focus) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;

  
  virtual content::WebContents* GetWebContents() OVERRIDE;
  virtual ToolbarModel* GetToolbarModel() OVERRIDE;
  virtual const ToolbarModel* GetToolbarModel() const OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual views::Widget* CreateViewsBubble(
      views::BubbleDelegateView* bubble_delegate) OVERRIDE;
  virtual PageActionImageView* CreatePageActionImageView(
      LocationBarView* owner, ExtensionAction* action) OVERRIDE;
  virtual ContentSettingBubbleModelDelegate*
      GetContentSettingBubbleModelDelegate() OVERRIDE;
  virtual void ShowWebsiteSettings(content::WebContents* web_contents,
                                   const GURL& url,
                                   const content::SSLStatus& ssl) OVERRIDE;

  
  virtual void EnabledStateChangedForCommand(int id, bool enabled) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnWidgetVisibilityChanged(views::Widget* widget,
                                         bool visible) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual bool GetAcceleratorForCommandId(
      int command_id, ui::Accelerator* accelerator) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& acc) OVERRIDE;

  
  bool IsWrenchMenuShowing() const;

  
  
  bool ShouldPaintBackground() const;

  enum {
    
    
    kStandardSpacing = 3,

    
    
    kVertSpacing = 5,
  };

 protected:
  
  virtual bool SetPaneFocusAndFocusDefault() OVERRIDE;
  virtual void RemovePaneFocus() OVERRIDE;

 private:
  
  enum DisplayMode {
    DISPLAYMODE_NORMAL,       
    DISPLAYMODE_LOCATION      
                              
  };

  
  bool ShouldShowUpgradeRecommended();

  
  bool ShouldShowBackgroundPageBadge();

  
  bool ShouldShowIncompatibilityWarning();

  
  int PopupTopSpacing() const;

  
  void LoadImages();

  bool is_display_mode_normal() const {
    return display_mode_ == DISPLAYMODE_NORMAL;
  }

  
  void ShowCriticalNotification();

  
  void ShowOutdatedInstallNotification();

  
  void UpdateAppMenuState();

  
  void UpdateWrenchButtonSeverity();

  void OnShowHomeButtonChanged();

  int content_shadow_height() const;

  
  BackButton* back_;
  ToolbarButton* forward_;
  ReloadButton* reload_;
  HomeButton* home_;
  LocationBarView* location_bar_;
  SiteChipView* site_chip_view_;
  BrowserActionsContainer* browser_actions_;
  WrenchToolbarButton* app_menu_;
  Browser* browser_;

  
  BooleanPrefMember show_home_button_;

  
  DisplayMode display_mode_;

  
  
  
  scoped_ptr<WrenchMenuModel> wrench_menu_model_;
  scoped_ptr<WrenchMenu> wrench_menu_;

  
  ObserverList<views::MenuListener> menu_listeners_;

  content::NotificationRegistrar registrar_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ToolbarView);
};

#endif  
