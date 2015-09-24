// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_VIEWS_LOGIN_DISPLAY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_VIEWS_LOGIN_DISPLAY_H_
#pragma once

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "chrome/browser/chromeos/login/message_bubble.h"
#include "chrome/browser/chromeos/login/user_controller.h"
#include "ui/gfx/rect.h"

namespace chromeos {

class HelpAppLauncher;
class MessageBubble;

class ViewsLoginDisplay : public LoginDisplay,
                          public UserController::Delegate,
                          public MessageBubbleDelegate {
 public:
  ViewsLoginDisplay(LoginDisplay::Delegate* delegate,
                    const gfx::Rect& background_bounds);
  virtual ~ViewsLoginDisplay();

  
  virtual void Init(const std::vector<UserManager::User>& users,
                    bool show_guest,
                    bool show_new_user);
  virtual void OnBeforeUserRemoved(const std::string& username);
  virtual void OnUserImageChanged(UserManager::User* user);
  virtual void OnUserRemoved(const std::string& username);
  virtual void OnFadeOut();
  virtual void SetUIEnabled(bool is_enabled);
  virtual void ShowError(int error_msg_id,
                         int login_attempts,
                         HelpAppLauncher::HelpTopic help_topic_id);

  
  virtual void CreateAccount() OVERRIDE;
  virtual void Login(UserController* source, const string16& password) OVERRIDE;
  virtual void LoginAsGuest() OVERRIDE;
  virtual void ClearErrors() OVERRIDE;
  virtual void OnUserSelected(UserController* source) OVERRIDE;
  virtual void RemoveUser(UserController* source) OVERRIDE;
  virtual void SelectUser(int index) OVERRIDE;
  virtual void StartEnterpriseEnrollment() OVERRIDE;

  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape) {
    bubble_ = NULL;
  }
  virtual bool CloseOnEscape() { return true; }
  virtual bool FadeInOnShow() { return false; }
  virtual void OnHelpLinkActivated();

 private:
  
  
  UserController* GetUserControllerByEmail(const std::string& email);

  
  
  MessageBubble* bubble_;

  
  
  
  UserController* controller_for_removal_;

  
  std::vector<UserController*> controllers_;

  
  int error_msg_id_;

  
  scoped_refptr<HelpAppLauncher> help_app_;

  
  HelpAppLauncher::HelpTopic help_topic_id_;

  
  std::vector<UserController*> invisible_controllers_;

  
  size_t selected_view_index_;

  DISALLOW_COPY_AND_ASSIGN(ViewsLoginDisplay);
};

}  

#endif  
