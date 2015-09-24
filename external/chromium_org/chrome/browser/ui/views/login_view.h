// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOGIN_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOGIN_VIEW_H_

#include "chrome/browser/ui/login/login_model.h"
#include "ui/views/view.h"

namespace views {
class Label;
class Textfield;
class LoginModel;
}  

class LoginView : public views::View, public LoginModelObserver {
 public:
  
  
  
  LoginView(const base::string16& explanation,
            LoginModel* model);
  virtual ~LoginView();

  
  base::string16 GetUsername();
  base::string16 GetPassword();

  
  virtual void OnAutofillDataAvailable(const base::string16& username,
                                       const base::string16& password) OVERRIDE;
  virtual void OnLoginModelDestroying() OVERRIDE;

  
  views::View* GetInitiallyFocusedView();

 private:
  
  views::Textfield* username_field_;
  views::Textfield* password_field_;

  
  views::Label* username_label_;
  views::Label* password_label_;

  
  views::Label* message_label_;

  
  
  LoginModel* login_model_;

  DISALLOW_COPY_AND_ASSIGN(LoginView);
};

#endif  
