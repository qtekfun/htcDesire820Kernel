// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_CONTROLLER_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"
#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/chromeos/login/new_user_view.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/chromeos/login/user_view.h"
#include "chrome/browser/chromeos/wm_ipc.h"
#include "testing/gtest/include/gtest/gtest_prod.h"
#include "views/controls/button/button.h"
#include "views/controls/textfield/textfield.h"
#include "views/widget/widget_delegate.h"

namespace views {
class WidgetGtk;
}

namespace chromeos {

class ThrobberManager;

class UserController : public views::WidgetDelegate,
                       public NewUserView::Delegate,
                       public UserView::Delegate {
 public:
  class Delegate {
   public:
    virtual void CreateAccount() = 0;
    virtual void Login(UserController* source,
                       const string16& password) = 0;
    virtual void LoginAsGuest() = 0;
    virtual void ClearErrors() = 0;
    virtual void OnUserSelected(UserController* source) = 0;
    virtual void RemoveUser(UserController* source) = 0;

    
    
    virtual void SelectUser(int index) = 0;

    
    virtual void StartEnterpriseEnrollment() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  UserController(Delegate* delegate, bool is_guest);

  
  UserController(Delegate* delegate, const UserManager::User& user);

  ~UserController();

  
  
  
  void Init(int index, int total_user_count, bool need_browse_without_signin);

  int user_index() const { return user_index_; }
  bool is_new_user() const { return is_new_user_; }
  bool is_guest() const { return is_guest_; }
  bool is_owner() const { return is_owner_; }

  const UserManager::User& user() const { return user_; }

  
  views::WidgetGtk* controls_window() {
    return controls_window_;
  }

  
  void ClearAndEnableFields();

  
  void ClearAndEnablePassword();

  
  void EnableNameTooltip(bool enable);

  
  void OnUserImageChanged(UserManager::User* user);

  
  
  gfx::Rect GetMainInputScreenBounds() const;

  
  void SelectUserRelative(int shift);

  
  void StartThrobber();
  void StopThrobber();

  
  
  void UpdateUserCount(int index, int total_user_count);

  
  
  std::string GetAccessibleUserLabel();

  
  virtual void OnWidgetActivated(bool active) OVERRIDE;

  
  virtual void OnLogin(const std::string& username,
                       const std::string& password) OVERRIDE;
  virtual void OnLoginAsGuest() OVERRIDE;
  virtual void OnCreateAccount() OVERRIDE;
  virtual void OnStartEnterpriseEnrollment() OVERRIDE;
  virtual void ClearErrors() OVERRIDE;
  virtual void NavigateAway() OVERRIDE;

  
  virtual void OnRemoveUser() OVERRIDE;
  virtual bool IsUserSelected() const OVERRIDE { return is_user_selected_; }

  
  virtual void OnLocaleChanged() OVERRIDE;

  
  static const int kPadding;

  
  static const int kUnselectedSize;
  static const int kNewUserUnselectedSize;

 private:
  FRIEND_TEST(UserControllerTest, GetNameTooltip);

  
  void ConfigureLoginWindow(views::WidgetGtk* window,
                            int index,
                            const gfx::Rect& bounds,
                            chromeos::WmIpcWindowType type,
                            views::View* contents_view);
  views::WidgetGtk* CreateControlsWindow(int index,
                                         int* width, int* height,
                                         bool need_guest_link);
  views::WidgetGtk* CreateImageWindow(int index);
  views::WidgetGtk* CreateLabelWindow(int index, WmIpcWindowType type);
  gfx::Font GetLabelFont();
  gfx::Font GetUnselectedLabelFont();
  void CreateBorderWindow(int index,
                          int total_user_count,
                          int controls_width, int controls_height);

  
  std::wstring GetNameTooltip() const;

  
  int user_index_;

  
  bool is_user_selected_;

  
  const bool is_new_user_;

  
  const bool is_guest_;

  
  const bool is_owner_;

  
  
  bool show_name_tooltip_;

  
  UserManager::User user_;

  Delegate* delegate_;

  
  views::WidgetGtk* controls_window_;
  views::WidgetGtk* image_window_;
  views::Widget* border_window_;
  views::WidgetGtk* label_window_;
  views::WidgetGtk* unselected_label_window_;

  
  UserView* user_view_;

  
  views::Label* label_view_;
  views::Label* unselected_label_view_;

  
  UserInput* user_input_;

  
  ThrobberHostView* throbber_host_;

  
  bool name_tooltip_enabled_;

  DISALLOW_COPY_AND_ASSIGN(UserController);
};

}  

#endif  
