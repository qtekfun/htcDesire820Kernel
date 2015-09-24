// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOGIN_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOGIN_VIEW_H_
#pragma once

#include "base/task.h"
#include "chrome/browser/ui/login/login_model.h"
#include "views/view.h"

namespace views {
class Label;
class Textfield;
class LoginModel;
}  

class LoginView : public views::View, public LoginModelObserver {
 public:
  
  
  
  LoginView(const std::wstring& explanation, LoginModel* model);
  virtual ~LoginView();

  
  std::wstring GetUsername();
  std::wstring GetPassword();

  
  virtual void OnAutofillDataAvailable(const std::wstring& username,
                                       const std::wstring& password) OVERRIDE;

  
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
