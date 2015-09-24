// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_NEW_AVATAR_BUTTON_H_
#define CHROME_BROWSER_UI_VIEWS_NEW_AVATAR_BUTTON_H_

#include "chrome/browser/profiles/avatar_menu.h"
#include "chrome/browser/profiles/avatar_menu_observer.h"
#include "ui/views/controls/button/menu_button.h"

class NewAvatarButton : public views::MenuButton,
                        public AvatarMenuObserver {
 public:
  
  enum AvatarButtonStyle {
    THEMED_BUTTON,   
    NATIVE_BUTTON,    
  };

  NewAvatarButton(views::ButtonListener* listener,
                  const base::string16& profile_name,
                  AvatarButtonStyle button_style,
                  Browser* browser);
  virtual ~NewAvatarButton();

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  friend class NewAvatarMenuButtonTest;
  FRIEND_TEST_ALL_PREFIXES(NewAvatarMenuButtonTest, SignOut);

  
  virtual void OnAvatarMenuChanged(AvatarMenu* avatar_menu) OVERRIDE;

  scoped_ptr<AvatarMenu> avatar_menu_;
  Browser* browser_;

  DISALLOW_COPY_AND_ASSIGN(NewAvatarButton);
};

#endif  
