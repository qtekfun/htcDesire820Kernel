// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_PERFORMER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_PERFORMER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "chrome/browser/chromeos/login/authenticator.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/browser/chromeos/login/signed_settings_helper.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace chromeos {

class LoginPerformer : public LoginStatusConsumer,
                       public SignedSettingsHelper::Callback,
                       public NotificationObserver,
                       public ProfileManager::Observer {
 public:
  
  class Delegate : public LoginStatusConsumer {
   public:
    virtual ~Delegate() {}
    virtual void WhiteListCheckFailed(const std::string& email) = 0;
  };

  explicit LoginPerformer(Delegate* delegate);
  virtual ~LoginPerformer();

  
  
  
  static LoginPerformer* default_performer() {
    return default_performer_;
  }

  
  virtual void OnLoginFailure(const LoginFailure& error);
  virtual void OnLoginSuccess(
      const std::string& username,
      const std::string& password,
      const GaiaAuthConsumer::ClientLoginResult& credentials,
      bool pending_requests);
  virtual void OnOffTheRecordLoginSuccess();
  virtual void OnPasswordChangeDetected(
      const GaiaAuthConsumer::ClientLoginResult& credentials);

  
  virtual void OnCheckWhitelistCompleted(SignedSettings::ReturnCode code,
                                         const std::string& email);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void Login(const std::string& username, const std::string& password);

  
  void LoginOffTheRecord();

  
  void RecoverEncryptedData(const std::string& old_password);

  
  void ResyncEncryptedData();

  
  const GoogleServiceAuthError& error() const {
    return last_login_failure_.error();
  }

  
  bool login_timed_out() {
    return last_login_failure_.reason() == LoginFailure::LOGIN_TIMED_OUT;
  }

  void set_captcha(const std::string& captcha) { captcha_ = captcha; }
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

 private:
  
  void OnProfileCreated(Profile* profile);

  
  void RequestScreenLock();

  
  void RequestScreenUnlock();

  
  
  void ResolveInitialNetworkAuthFailure();

  
  void ResolveLockLoginFailure();

  
  
  void ResolveLockNetworkAuthFailure();

  
  void ResolveScreenLocked();
  void ResolveScreenUnlocked();

  
  void StartAuthentication();

  
  static LoginPerformer* default_performer_;

  
  scoped_refptr<Authenticator> authenticator_;

  
  
  LoginFailure last_login_failure_;

  
  std::string captcha_;

  
  std::string captcha_token_;

  
  GaiaAuthConsumer::ClientLoginResult cached_credentials_;

  
  std::string username_;
  std::string password_;

  
  Delegate* delegate_;

  
  
  bool password_changed_;

  
  NotificationRegistrar registrar_;

  
  
  bool screen_lock_requested_;

  
  
  
  bool initial_online_auth_pending_;

  GaiaAuthConsumer::ClientLoginResult credentials_;

  ScopedRunnableMethodFactory<LoginPerformer> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(LoginPerformer);
};

}  

#endif  
