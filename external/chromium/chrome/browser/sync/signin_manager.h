// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SIGNIN_MANAGER_H_
#define CHROME_BROWSER_SYNC_SIGNIN_MANAGER_H_
#pragma once

#include <string>
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"

class GaiaAuthFetcher;
class Profile;
class PrefService;

struct GoogleServiceSigninSuccessDetails {
  GoogleServiceSigninSuccessDetails(const std::string& in_username,
                                    const std::string& in_password)
      : username(in_username),
        password(in_password) {}
  std::string username;
  std::string password;
};

class SigninManager : public GaiaAuthConsumer {
 public:
  SigninManager();
  virtual ~SigninManager();

  
  static void RegisterUserPrefs(PrefService* user_prefs);

  
  void Initialize(Profile* profile);

  
  
  const std::string& GetUsername();

  
  void SetUsername(const std::string& username);

  
  
  
  void StartSignIn(const std::string& username,
                   const std::string& password,
                   const std::string& login_token,
                   const std::string& login_captcha);

  
  
  void ProvideSecondFactorAccessCode(const std::string& access_code);

  
  
  void SignOut();

  
  virtual void OnClientLoginSuccess(const ClientLoginResult& result);
  virtual void OnClientLoginFailure(const GoogleServiceAuthError& error);
  virtual void OnGetUserInfoSuccess(const std::string& key,
                                    const std::string& value);
  virtual void OnGetUserInfoKeyNotFound(const std::string& key);
  virtual void OnGetUserInfoFailure(const GoogleServiceAuthError& error);

 private:
  Profile* profile_;
  std::string username_;
  std::string password_;  
  bool had_two_factor_error_;

  
  
  ClientLoginResult last_result_;

  
  scoped_ptr<GaiaAuthFetcher> client_login_;
};

#endif  
