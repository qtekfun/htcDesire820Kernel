// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_ATTEMPT_STATE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_ATTEMPT_STATE_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_fetcher.h"

namespace chromeos {


class AuthAttemptState {
 public:
  
  AuthAttemptState(const std::string& username,
                   const std::string& password,
                   const std::string& ascii_hash,
                   const std::string& login_token,
                   const std::string& login_captcha,
                   const bool user_is_new);

  
  AuthAttemptState(const std::string& username, const std::string& ascii_hash);

  virtual ~AuthAttemptState();

  
  
  
  void RecordOnlineLoginStatus(
      const GaiaAuthConsumer::ClientLoginResult& credentials,
      const LoginFailure& outcome);

  
  void DisableHosted();

  
  
  
  void RecordCryptohomeStatus(bool cryptohome_outcome, int cryptohome_code);

  
  
  void ResetCryptohomeStatus();

  virtual bool online_complete();
  virtual const LoginFailure& online_outcome();
  virtual const GaiaAuthConsumer::ClientLoginResult& credentials();
  virtual bool is_first_time_user();
  virtual GaiaAuthFetcher::HostedAccountsSetting hosted_policy();

  virtual bool cryptohome_complete();
  virtual bool cryptohome_outcome();
  virtual int cryptohome_code();

  
  
  const std::string username;

  
  const std::string password;
  const std::string ascii_hash;
  const std::string login_token;
  const std::string login_captcha;

  const bool unlock;  

 protected:
  bool try_again_;  

  
  bool online_complete_;
  LoginFailure online_outcome_;
  GaiaAuthConsumer::ClientLoginResult credentials_;

  
  
  GaiaAuthFetcher::HostedAccountsSetting hosted_policy_;
  bool is_first_time_user_;

  
  bool cryptohome_complete_;
  bool cryptohome_outcome_;
  int cryptohome_code_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AuthAttemptState);
};

}  

#endif  
