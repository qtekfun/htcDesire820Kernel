// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_VIEW_H_
#pragma once

#include <string>

#include "views/controls/button/button.h"
#include "views/controls/link.h"
#include "views/view.h"

class SkBitmap;

namespace views {
class ImageView;
class TextButton;
class Throbber;
}  

namespace chromeos {

class SignoutView;
class PodImageView;

class UserView : public views::View,
                 public views::LinkController,
                 public views::ButtonListener {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnSignout() {}

    
    virtual void OnRemoveUser() {}

    
    virtual bool IsUserSelected() const = 0;

    
    virtual void OnLocaleChanged() {}
  };

  
  
  
  
  UserView(Delegate* delegate, bool is_login, bool need_background);

  
  virtual gfx::Size GetPreferredSize();
  virtual void OnLocaleChanged();

  
  
  
  
  void SetImage(const SkBitmap& image, const SkBitmap& image_hot);

  
  void SetTooltipText(const std::wstring& text);

  
  void SetRemoveButtonVisible(bool flag);

  
  void SetSignoutEnabled(bool enabled);

  
  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

 private:
  void Init(bool need_background);

  Delegate* delegate_;

  SignoutView* signout_view_;
  PodImageView* image_view_;

  views::TextButton* remove_button_;

  DISALLOW_COPY_AND_ASSIGN(UserView);
};

}  

#endif  
