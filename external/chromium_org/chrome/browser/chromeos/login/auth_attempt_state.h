// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_ATTEMPT_STATE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_ATTEMPT_STATE_H_

#include <string>

#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/browser/chromeos/login/user.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/gaia_auth_fetcher.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

namespace chromeos {


class AuthAttemptState {
 public:
  
  AuthAttemptState(const UserContext& user_context,
                   const std::string& login_token,
                   const std::string& login_captcha,
                   const User::UserType user_type,
                   const bool user_is_new);

  
  AuthAttemptState(const UserContext& user_context,
                   const bool user_is_new);

  
  AuthAttemptState(const std::string& username, const std::string& password);

  virtual ~AuthAttemptState();

  
  
  
  void RecordOnlineLoginStatus(
      const LoginFailure& outcome);

  
  
  
  void RecordUsernameHash(const std::string& username_hash);

  
  void RecordUsernameHashFailed();

  
  
  void UsernameHashRequested();

  
  void DisableHosted();

  
  
  
  void RecordCryptohomeStatus(bool cryptohome_outcome,
                              cryptohome::MountError cryptohome_code);

  
  
  void ResetCryptohomeStatus();

  virtual bool online_complete();
  virtual const LoginFailure& online_outcome();
  virtual bool is_first_time_user();
  virtual GaiaAuthFetcher::HostedAccountsSetting hosted_policy();

  virtual bool cryptohome_complete();
  virtual bool cryptohome_outcome();
  virtual cryptohome::MountError cryptohome_code();

  virtual bool username_hash_obtained();
  virtual bool username_hash_valid();

  
  
  UserContext user_context;

  
  const std::string login_token;
  const std::string login_captcha;

  
  const User::UserType user_type;

  const bool unlock;  

 protected:
  
  bool online_complete_;
  LoginFailure online_outcome_;

  
  
  GaiaAuthFetcher::HostedAccountsSetting hosted_policy_;
  bool is_first_time_user_;

  
  bool cryptohome_complete_;
  bool cryptohome_outcome_;
  cryptohome::MountError cryptohome_code_;

 private:
  
  
  
  bool username_hash_obtained_;

  
  
  bool username_hash_valid_;

  DISALLOW_COPY_AND_ASSIGN(AuthAttemptState);
};

}  

#endif  
