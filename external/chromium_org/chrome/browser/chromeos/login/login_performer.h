// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_PERFORMER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_PERFORMER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/login/authenticator.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/browser/chromeos/login/online_attempt_host.h"
#include "chrome/browser/chromeos/login/user.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "google_apis/gaia/google_service_auth_error.h"

namespace policy {
class WildcardLoginChecker;
}

namespace chromeos {

class LoginPerformer : public LoginStatusConsumer,
                       public OnlineAttemptHost::Delegate {
 public:
  typedef enum AuthorizationMode {
    
    AUTH_MODE_INTERNAL,
    
    AUTH_MODE_EXTENSION
  } AuthorizationMode;

  
  class Delegate : public LoginStatusConsumer {
   public:
    virtual ~Delegate() {}
    virtual void WhiteListCheckFailed(const std::string& email) = 0;
    virtual void PolicyLoadFailed() = 0;
    virtual void OnOnlineChecked(const std::string& email, bool success) = 0;
  };

  explicit LoginPerformer(Delegate* delegate);
  virtual ~LoginPerformer();

  
  virtual void OnLoginFailure(const LoginFailure& error) OVERRIDE;
  virtual void OnRetailModeLoginSuccess(
      const UserContext& user_context) OVERRIDE;
  virtual void OnLoginSuccess(const UserContext& user_context) OVERRIDE;
  virtual void OnOffTheRecordLoginSuccess() OVERRIDE;
  virtual void OnPasswordChangeDetected() OVERRIDE;

  
  
  
  void PerformLogin(const UserContext& user_context,
                    AuthorizationMode auth_mode);

  
  void LoginAsLocallyManagedUser(const UserContext& user_context);

  
  void LoginRetailMode();

  
  void LoginOffTheRecord();

  
  void LoginAsPublicAccount(const std::string& username);

  
  void LoginAsKioskAccount(const std::string& app_user_id);

  
  void RecoverEncryptedData(const std::string& old_password);

  
  void ResyncEncryptedData();

  
  const GoogleServiceAuthError& error() const {
    return last_login_failure_.error();
  }

  
  bool password_changed() { return password_changed_; }

  
  
  
  
  
  int password_changed_callback_count() {
    return password_changed_callback_count_;
  }

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

  AuthorizationMode auth_mode() const { return auth_mode_; }

 protected:
  
  virtual void OnChecked(const std::string& username, bool success) OVERRIDE;

 private:
  
  void StartLoginCompletion();

  
  void StartAuthentication();

  
  
  
  void OnlineWildcardLoginCheckCompleted(bool result);

  
  scoped_refptr<Authenticator> authenticator_;

  
  OnlineAttemptHost online_attempt_host_;

  
  
  LoginFailure last_login_failure_;

  
  UserContext user_context_;

  
  Delegate* delegate_;

  
  
  bool password_changed_;
  int password_changed_callback_count_;

  
  AuthorizationMode auth_mode_;

  
  scoped_ptr<policy::WildcardLoginChecker> wildcard_login_checker_;

  base::WeakPtrFactory<LoginPerformer> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(LoginPerformer);
};

}  

#endif  
