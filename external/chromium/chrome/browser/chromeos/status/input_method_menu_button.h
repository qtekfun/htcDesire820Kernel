// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_INPUT_METHOD_MENU_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_INPUT_METHOD_MENU_BUTTON_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/status/input_method_menu.h"
#include "chrome/browser/chromeos/status/status_area_button.h"
#include "views/controls/menu/view_menu_delegate.h"

namespace chromeos {

class StatusAreaHost;

class InputMethodMenuButton : public StatusAreaButton,
                              public views::ViewMenuDelegate {
 public:
  explicit InputMethodMenuButton(StatusAreaHost* host);
  virtual ~InputMethodMenuButton() {}

  
  virtual gfx::Size GetPreferredSize();
  virtual void OnLocaleChanged();

  
  virtual void RunMenu(views::View* unused_source, const gfx::Point& pt);

  
  virtual void UpdateUI(const std::string& input_method_id,
                        const std::wstring& name,
                        const std::wstring& tooltip,
                        size_t num_active_input_methods);
  virtual bool ShouldSupportConfigUI();
  virtual void OpenConfigUI();

  
  void UpdateUIFromCurrentInputMethod();

 private:
  
  bool WindowIsActive();

  scoped_ptr<InputMethodMenu> menu_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodMenuButton);
};

}  

#endif  
