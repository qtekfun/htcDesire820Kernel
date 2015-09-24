// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_STATUS_AREA_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_STATUS_AREA_BUTTON_H_
#pragma once

#include "chrome/browser/chromeos/status/status_area_host.h"
#include "views/controls/button/menu_button.h"
#include "views/controls/menu/view_menu_delegate.h"

namespace chromeos {

class StatusAreaButton : public views::MenuButton {
 public:
  explicit StatusAreaButton(StatusAreaHost* host,
                            views::ViewMenuDelegate* menu_delegate);
  virtual ~StatusAreaButton() {}
  virtual void PaintButton(gfx::Canvas* canvas, PaintButtonMode mode);

  
  
  virtual void SetText(const std::wstring& text);

  void set_use_menu_button_paint(bool use_menu_button_paint) {
    use_menu_button_paint_ = use_menu_button_paint;
  }

  
  virtual bool Activate() OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

  
  void set_active(bool active) { active_ = active; }
  bool active() const { return active_; }

 protected:
  
  virtual int icon_height() { return 24; }
  virtual int icon_width() { return 23; }

  
  
  virtual int horizontal_padding() { return 1; }

  
  bool use_menu_button_paint_;

  
  gfx::Insets insets_;

  
  
  
  bool active_;

  
  StatusAreaHost* host_;

 private:
  void UpdateTextStyle();

  DISALLOW_COPY_AND_ASSIGN(StatusAreaButton);
};

}  

#endif  
