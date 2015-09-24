// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AVATAR_MENU_BUTTON_H_
#define CHROME_BROWSER_UI_VIEWS_AVATAR_MENU_BUTTON_H_

#include <string>

#include "base/compiler_specific.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/button/menu_button_listener.h"

namespace gfx {
class Canvas;
class Image;
}
class Browser;


class AvatarMenuButton : public views::MenuButton,
                         public views::MenuButtonListener {
 public:
  
  static const char kViewClassName[];

  
  
  AvatarMenuButton(Browser* browser, bool disabled);

  virtual ~AvatarMenuButton();

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;

  
  
  virtual void SetAvatarIcon(const gfx::Image& icon, bool is_rectangle);

  void set_button_on_right(bool button_on_right) {
    button_on_right_ = button_on_right;
  }
  bool button_on_right() { return button_on_right_; }

  void ShowAvatarBubble();

 private:
  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;

  Browser* browser_;
  bool disabled_;
  scoped_ptr<ui::MenuModel> menu_model_;

  
  scoped_ptr<gfx::Image> icon_;
  gfx::ImageSkia button_icon_;
  bool is_rectangle_;
  int old_height_;
  
  bool button_on_right_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuButton);
};

#endif  
