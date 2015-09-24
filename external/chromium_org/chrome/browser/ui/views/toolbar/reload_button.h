// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_RELOAD_BUTTON_H__
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_RELOAD_BUTTON_H__

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/views/toolbar/toolbar_button.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/views/controls/button/button.h"

class CommandUpdater;
class LocationBarView;


class ReloadButton : public ToolbarButton,
                     public views::ButtonListener,
                     public ui::SimpleMenuModel::Delegate {
 public:
  enum Mode { MODE_RELOAD = 0, MODE_STOP };

  
  static const char kViewClassName[];

  ReloadButton(LocationBarView* location_bar,
               CommandUpdater* command_updater);
  virtual ~ReloadButton();

  
  
  void ChangeMode(Mode mode, bool force);

  
  void set_menu_enabled(bool enable) { menu_enabled_ = enable; }

  void LoadImages();

  
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              base::string16* tooltip) const OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool ShouldShowMenu() OVERRIDE;
  virtual void ShowDropDownMenu(ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* ,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdVisible(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  friend class ReloadButtonTest;

  ui::SimpleMenuModel* CreateMenuModel();

  void ExecuteBrowserCommand(int command, int event_flags);
  void ChangeModeInternal(Mode mode);

  void OnDoubleClickTimer();
  void OnStopToReloadTimer();

  base::OneShotTimer<ReloadButton> double_click_timer_;
  base::OneShotTimer<ReloadButton> stop_to_reload_timer_;

  
  LocationBarView* location_bar_;
  CommandUpdater* command_updater_;

  
  Mode intended_mode_;

  
  Mode visible_mode_;

  
  
  base::TimeDelta double_click_timer_delay_;
  base::TimeDelta stop_to_reload_timer_delay_;

  
  bool menu_enabled_;

  
  
  bool testing_mouse_hovered_;
  
  
  int testing_reload_count_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ReloadButton);
};

#endif  
