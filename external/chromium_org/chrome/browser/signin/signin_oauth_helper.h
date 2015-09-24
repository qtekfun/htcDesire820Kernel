// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_OAUTH_HELPER_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_OAUTH_HELPER_H_

#include "base/memory/scoped_ptr.h"
#include "google_apis/gaia/gaia_auth_consumer.h"

class GaiaAuthFetcher;
class Profile;

class SigninOAuthHelper : public GaiaAuthConsumer {
 public:
  explicit SigninOAuthHelper(Profile* profile);
  virtual ~SigninOAuthHelper();

  void StartAddingAccount(const std::string& oauth_code);

  
  virtual void OnClientOAuthSuccess(const ClientOAuthResult& result) OVERRIDE;
  virtual void OnClientOAuthFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnClientLoginSuccess(const ClientLoginResult& result) OVERRIDE;
  virtual void OnClientLoginFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnGetUserInfoSuccess(const UserInfoMap& data) OVERRIDE;
  virtual void OnGetUserInfoFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

 private:
  Profile* profile_;
  scoped_ptr<GaiaAuthFetcher> gaia_auth_fetcher_;
  std::string refresh_token_;

  DISALLOW_COPY_AND_ASSIGN(SigninOAuthHelper);
};

#endif  
