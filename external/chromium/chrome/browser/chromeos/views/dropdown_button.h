// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_VIEWS_DROPDOWN_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_VIEWS_DROPDOWN_BUTTON_H_
#pragma once

#include <string>

#include "views/controls/button/menu_button.h"

namespace chromeos {

class DropDownButton : public views::MenuButton {
 public:
  DropDownButton(views::ButtonListener* listener,
                 const std::wstring& text,
                 views::ViewMenuDelegate* menu_delegate,
                 bool show_menu_marker);
  virtual ~DropDownButton();

  virtual void OnPaintFocusBorder(gfx::Canvas* canvas);

  
  
  
  virtual void SetText(const std::wstring& text);

  virtual string16 GetAccessibleValue() { return text_; }

 private:
  DISALLOW_COPY_AND_ASSIGN(DropDownButton);
};

}  

#endif  
