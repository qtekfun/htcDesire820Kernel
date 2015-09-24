// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_PROFILE_OAUTH2_TOKEN_SERVICE_H_
#define CHROME_BROWSER_SIGNIN_PROFILE_OAUTH2_TOKEN_SERVICE_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "chrome/browser/signin/signin_global_error.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "google_apis/gaia/oauth2_token_service.h"

namespace net {
class URLRequestContextGetter;
}

class GoogleServiceAuthError;
class Profile;
class SigninGlobalError;

class ProfileOAuth2TokenService : public OAuth2TokenService,
                                  public BrowserContextKeyedService {
 public:
  
  virtual void Initialize(Profile* profile);

  
  
  
  
  
  virtual void LoadCredentials();

  
  virtual void Shutdown() OVERRIDE;

  
  std::string GetPrimaryAccountId();

  
  virtual std::vector<std::string> GetAccounts() OVERRIDE;

  
  
  virtual void UpdateCredentials(const std::string& account_id,
                                 const std::string& refresh_token);

  
  void RevokeCredentials(const std::string& account_id);

  
  void RevokeAllCredentials();

  SigninGlobalError* signin_global_error() {
    return signin_global_error_.get();
  }

  const SigninGlobalError* signin_global_error() const {
    return signin_global_error_.get();
  }

  Profile* profile() const { return profile_; }

 protected:
  class AccountInfo : public SigninGlobalError::AuthStatusProvider {
   public:
    AccountInfo(ProfileOAuth2TokenService* token_service,
                const std::string& account_id,
                const std::string& refresh_token);
    virtual ~AccountInfo();

    const std::string& refresh_token() const { return refresh_token_; }
    void set_refresh_token(const std::string& token) {
      refresh_token_ = token;
    }

    void SetLastAuthError(const GoogleServiceAuthError& error);

    
    virtual std::string GetAccountId() const OVERRIDE;
    virtual GoogleServiceAuthError GetAuthStatus() const OVERRIDE;

   private:
    ProfileOAuth2TokenService* token_service_;
    std::string account_id_;
    std::string refresh_token_;
    GoogleServiceAuthError last_auth_error_;

    DISALLOW_COPY_AND_ASSIGN(AccountInfo);
  };

  
  
  typedef std::map<std::string, linked_ptr<AccountInfo> > AccountInfoMap;

  ProfileOAuth2TokenService();
  virtual ~ProfileOAuth2TokenService();

  
  virtual std::string GetRefreshToken(const std::string& account_id) OVERRIDE;

  
  virtual net::URLRequestContextGetter* GetRequestContext() OVERRIDE;

  
  
  virtual void UpdateAuthError(
      const std::string& account_id,
      const GoogleServiceAuthError& error) OVERRIDE;

  
  
  virtual void PersistCredentials(const std::string& account_id,
                                  const std::string& refresh_token);

  
  
  virtual void ClearPersistedCredentials(const std::string& account_id);

  AccountInfoMap& refresh_tokens() { return refresh_tokens_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(MutableProfileOAuth2TokenServiceTest,
                           TokenServiceUpdateClearsCache);
  FRIEND_TEST_ALL_PREFIXES(MutableProfileOAuth2TokenServiceTest,
                           PersistenceDBUpgrade);
  FRIEND_TEST_ALL_PREFIXES(MutableProfileOAuth2TokenServiceTest,
                           PersistenceLoadCredentials);

  
  virtual void RevokeCredentialsOnServer(const std::string& refresh_token);

  
  Profile* profile_;

  
  AccountInfoMap refresh_tokens_;

  
  
  
  
  scoped_ptr<SigninGlobalError> signin_global_error_;

  DISALLOW_COPY_AND_ASSIGN(ProfileOAuth2TokenService);
};

#endif  
