// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_RELOAD_BUTTON_H__
#define CHROME_BROWSER_UI_VIEWS_RELOAD_BUTTON_H__
#pragma once

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/timer.h"
#include "views/controls/button/image_button.h"

class Browser;
class LocationBarView;


class ReloadButton : public views::ToggleImageButton,
                     public views::ButtonListener {
 public:
  enum Mode { MODE_RELOAD = 0, MODE_STOP };

  ReloadButton(LocationBarView* location_bar, Browser* Browser);
  virtual ~ReloadButton();

  
  
  void ChangeMode(Mode mode, bool force);

  
  virtual void ButtonPressed(views::Button* ,
                             const views::Event& event) OVERRIDE;

  
  virtual void OnMouseExited(const views::MouseEvent& event) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              std::wstring* tooltip) OVERRIDE;

 private:
  friend class ReloadButtonTest;

  void OnDoubleClickTimer();
  void OnStopToReloadTimer();

  base::OneShotTimer<ReloadButton> double_click_timer_;
  base::OneShotTimer<ReloadButton> stop_to_reload_timer_;

  
  LocationBarView* location_bar_;
  Browser* browser_;

  
  Mode intended_mode_;

  
  Mode visible_mode_;

  
  
  base::TimeDelta double_click_timer_delay_;
  base::TimeDelta stop_to_reload_timer_delay_;

  
  
  bool testing_mouse_hovered_;
  
  
  int testing_reload_count_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ReloadButton);
};

#endif  
