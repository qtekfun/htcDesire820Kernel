// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_DISPLAY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_DISPLAY_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/chromeos/login/help_app_launcher.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

namespace chromeos {

class RemoveUserDelegate {
 public:
  
  virtual void OnBeforeUserRemoved(const std::string& username) = 0;

  
  virtual void OnUserRemoved(const std::string& username) = 0;
};

class LoginDisplay : public RemoveUserDelegate {
 public:
  class Delegate {
   public:
    
    virtual void CreateAccount() = 0;

    
    virtual string16 GetConnectedNetworkName() = 0;

    
    virtual void FixCaptivePortal() = 0;

    
    
    virtual void Login(const std::string& username,
                       const std::string& password) = 0;

    
    virtual void LoginAsGuest() = 0;

    
    virtual void OnUserSelected(const std::string& username) = 0;

    
    virtual void OnStartEnterpriseEnrollment() = 0;

   protected:
    virtual ~Delegate();
  };

  
  LoginDisplay(Delegate* delegate, const gfx::Rect& background_bounds);
  virtual ~LoginDisplay();

  
  
  virtual void Destroy();

  
  
  virtual void Init(const std::vector<UserManager::User>& users,
                    bool show_guest,
                    bool show_new_user) = 0;


  
  
  virtual void OnUserImageChanged(UserManager::User* user) = 0;

  
  
  virtual void OnFadeOut() = 0;

  
  virtual void SetUIEnabled(bool is_enabled) = 0;

  
  
  
  virtual void ShowError(int error_msg_id,
                         int login_attempts,
                         HelpAppLauncher::HelpTopic help_topic_id) = 0;

  gfx::Rect background_bounds() const { return background_bounds_; }
  void set_background_bounds(const gfx::Rect background_bounds){
    background_bounds_ = background_bounds;
  }

  Delegate* delegate() { return delegate_; }
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

  gfx::NativeWindow parent_window() const { return parent_window_; }
  void set_parent_window(gfx::NativeWindow window) { parent_window_ = window; }

  int width() const { return background_bounds_.width(); }

 protected:
  
  Delegate* delegate_;

  
  gfx::NativeWindow parent_window_;

  
  gfx::Rect background_bounds_;

  DISALLOW_COPY_AND_ASSIGN(LoginDisplay);
};

}  

#endif  
