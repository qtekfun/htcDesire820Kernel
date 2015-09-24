// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_EXISTING_USER_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_EXISTING_USER_CONTROLLER_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/timer.h"
#include "chrome/browser/chromeos/login/captcha_view.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "chrome/browser/chromeos/login/login_performer.h"
#include "chrome/browser/chromeos/login/login_utils.h"
#include "chrome/browser/chromeos/login/ownership_status_checker.h"
#include "chrome/browser/chromeos/login/password_changed_view.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/chromeos/wm_message_listener.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "testing/gtest/include/gtest/gtest_prod.h"
#include "ui/gfx/rect.h"

namespace chromeos {

class LoginDisplayHost;
class UserCrosSettingsProvider;

class ExistingUserController : public LoginDisplay::Delegate,
                               public NotificationObserver,
                               public LoginPerformer::Delegate,
                               public LoginUtils::Delegate,
                               public CaptchaView::Delegate,
                               public PasswordChangedView::Delegate {
 public:
  
  explicit ExistingUserController(LoginDisplayHost* host);
  ~ExistingUserController();

  
  static ExistingUserController* current_controller() {
    return current_controller_;
  }

  
  void Init(const UserVector& users);

  
  virtual void CreateAccount() OVERRIDE;
  virtual string16 GetConnectedNetworkName() OVERRIDE;
  virtual void FixCaptivePortal() OVERRIDE;
  virtual void Login(const std::string& username,
                     const std::string& password) OVERRIDE;
  virtual void LoginAsGuest() OVERRIDE;
  virtual void OnUserSelected(const std::string& username) OVERRIDE;
  virtual void OnStartEnterpriseEnrollment() OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  friend class ExistingUserControllerTest;
  friend class MockLoginPerformerDelegate;

  
  virtual void OnLoginFailure(const LoginFailure& error);
  virtual void OnLoginSuccess(
      const std::string& username,
      const std::string& password,
      const GaiaAuthConsumer::ClientLoginResult& credentials,
      bool pending_requests);
  virtual void OnOffTheRecordLoginSuccess();
  virtual void OnPasswordChangeDetected(
      const GaiaAuthConsumer::ClientLoginResult& credentials);
  virtual void WhiteListCheckFailed(const std::string& email);

  
  virtual void OnProfilePrepared(Profile* profile);

  
  virtual void OnCaptchaEntered(const std::string& captcha);

  
  virtual void RecoverEncryptedData(const std::string& old_password);
  virtual void ResyncEncryptedData();

  
  void ActivateWizard(const std::string& screen_name);

  
  gfx::NativeWindow GetNativeWindow() const;

  
  void SetStatusAreaEnabled(bool enable);

  
  
  
  void ShowError(int error_id, const std::string& details);

  
  
  void OnEnrollmentOwnershipCheckCompleted(OwnershipService::Status status);

  void set_login_performer_delegate(LoginPerformer::Delegate* d) {
    login_performer_delegate_.reset(d);
  }

  
  scoped_ptr<LoginPerformer> login_performer_;

  
  LoginDisplay* login_display_;

  
  
  scoped_ptr<LoginPerformer::Delegate> login_performer_delegate_;

  
  std::string last_login_attempt_username_;

  
  LoginDisplayHost* host_;

  
  
  size_t num_login_attempts_;

  
  
  static ExistingUserController* current_controller_;

  
  scoped_ptr<UserCrosSettingsProvider> user_settings_;

  
  GURL guest_mode_url_;

  
  NotificationRegistrar registrar_;

  
  ScopedRunnableMethodFactory<ExistingUserController> method_factory_;

  
  bool ready_for_browser_launch_;

  
  bool two_factor_credentials_;

  
  scoped_ptr<OwnershipStatusChecker> ownership_checker_;

  FRIEND_TEST_ALL_PREFIXES(ExistingUserControllerTest, NewUserLogin);

  DISALLOW_COPY_AND_ASSIGN(ExistingUserController);
};

}  

#endif  
