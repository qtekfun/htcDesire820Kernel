// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_NEW_USER_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_NEW_USER_VIEW_H_
#pragma once

#include <string>

#include "base/task.h"
#include "chrome/browser/chromeos/login/helper.h"
#include "chrome/browser/chromeos/login/language_switch_menu.h"
#include "chrome/browser/chromeos/login/user_input.h"
#include "views/accelerator.h"
#include "views/controls/button/button.h"
#include "views/controls/link.h"
#include "views/controls/textfield/textfield_controller.h"
#include "views/view.h"

namespace views {
class Label;
class MenuButton;
class NativeButton;
}  

namespace chromeos {

class NewUserView : public ThrobberHostView,
                    public UserInput,
                    public views::TextfieldController,
                    public views::LinkController,
                    public views::ButtonListener {
 public:
  
  class Delegate {
  public:
    virtual ~Delegate() {}

    
    virtual void OnLogin(const std::string& username,
                         const std::string& password) = 0;

    
    virtual void OnLoginAsGuest() = 0;

    
    virtual void OnCreateAccount() = 0;

    
    virtual void OnStartEnterpriseEnrollment() = 0;

    
    virtual void ClearErrors() = 0;

    
    virtual void NavigateAway() = 0;
  };

  
  NewUserView(Delegate* delegate,
              bool need_border,
              bool need_guest_link);

  virtual ~NewUserView();

  
  void Init();

  
  void UpdateLocalizedStringsAndFonts();

  
  gfx::Rect GetPasswordBounds() const;

  
  gfx::Rect GetUsernameBounds() const;

  
  virtual gfx::Size GetPreferredSize();
  virtual void Layout();
  virtual void RequestFocus();

  
  void SetUsername(const std::string& username);
  void SetPassword(const std::string& password);

  
  void Login();

  
  
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& keystroke);
  virtual void ContentsChanged(views::Textfield* sender,
                               const string16& new_contents);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual void LinkActivated(views::Link* source, int event_flags);
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  
  virtual gfx::Rect CalculateThrobberBounds(views::Throbber* throbber);

  
  virtual void EnableInputControls(bool enabled);
  virtual void ClearAndFocusControls();
  virtual void ClearAndFocusPassword();
  virtual gfx::Rect GetMainInputScreenBounds() const;

  
  
  bool NavigateAway();

 protected:
  
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View *parent,
                                    views::View *child);
  virtual void OnLocaleChanged();
  void AddChildView(View* view);

 private:
  
  void InitLink(views::Link** link);

  
  
  void RecreatePeculiarControls();

  
  
  
  void UpdateSignInButtonState();

  
  views::View* CreateSplitter(SkColor color);

  
  
  
  
  views::Textfield* username_field_;
  views::Textfield* password_field_;
  views::Label* title_label_;
  views::Label* title_hint_label_;
  views::View* splitter_up1_;
  views::View* splitter_up2_;
  views::View* splitter_down1_;
  views::View* splitter_down2_;
  views::NativeButton* sign_in_button_;
  views::Link* guest_link_;
  views::Link* create_account_link_;
  views::MenuButton* languages_menubutton_;

  views::Accelerator accel_focus_pass_;
  views::Accelerator accel_focus_user_;
  views::Accelerator accel_enterprise_enrollment_;
  views::Accelerator accel_login_off_the_record_;
  views::Accelerator accel_toggle_accessibility_;

  
  Delegate* delegate_;

  ScopedRunnableMethodFactory<NewUserView> focus_grabber_factory_;

  LanguageSwitchMenu language_switch_menu_;

  
  bool login_in_process_;

  
  bool need_border_;

  
  bool need_guest_link_;

  
  
  bool need_create_account_;

  
  int languages_menubutton_order_;
  int sign_in_button_order_;

  DISALLOW_COPY_AND_ASSIGN(NewUserView);
};

}  

#endif  
