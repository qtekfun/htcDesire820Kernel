// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OAUTH2_LOGIN_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OAUTH2_LOGIN_MANAGER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/login/oauth2_login_verifier.h"
#include "chrome/browser/chromeos/login/oauth2_token_fetcher.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "google_apis/gaia/gaia_oauth_client.h"
#include "google_apis/gaia/oauth2_token_service.h"
#include "net/url_request/url_request_context_getter.h"

class GoogleServiceAuthError;
class Profile;
class ProfileOAuth2TokenService;

namespace chromeos {

class OAuth2LoginManager : public BrowserContextKeyedService,
                           public gaia::GaiaOAuthClient::Delegate,
                           public OAuth2LoginVerifier::Delegate,
                           public OAuth2TokenFetcher::Delegate,
                           public OAuth2TokenService::Observer {
 public:
  
  enum SessionRestoreState {
    
    SESSION_RESTORE_NOT_STARTED,
    
    SESSION_RESTORE_PREPARING,
    
    
    SESSION_RESTORE_IN_PROGRESS,
    
    SESSION_RESTORE_DONE,
    
    SESSION_RESTORE_FAILED,
    
    SESSION_RESTORE_CONNECTION_FAILED,
  };

  
  enum SessionRestoreStrategy {
    
    
    RESTORE_FROM_COOKIE_JAR,
    
    RESTORE_FROM_SAVED_OAUTH2_REFRESH_TOKEN,
    
    RESTORE_FROM_PASSED_OAUTH2_REFRESH_TOKEN,
    
    RESTORE_FROM_AUTH_CODE,
  };

  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void OnSessionRestoreStateChanged(Profile* user_profile,
                                              SessionRestoreState state) {}

    
    virtual void OnNewRefreshTokenAvaiable(Profile* user_profile) {}

    
    
    virtual void OnSessionAuthenticated(Profile* user_profile) {}
  };

  explicit OAuth2LoginManager(Profile* user_profile);
  virtual ~OAuth2LoginManager();

  void AddObserver(OAuth2LoginManager::Observer* observer);
  void RemoveObserver(OAuth2LoginManager::Observer* observer);

  
  
  
  
  
  
  void RestoreSession(
      net::URLRequestContextGetter* auth_request_context,
      SessionRestoreStrategy restore_strategy,
      const std::string& oauth2_refresh_token,
      const std::string& auth_code);

  
  void ContinueSessionRestore();

  
  void RestoreSessionFromSavedTokens();

  
  void Stop();

  
  SessionRestoreState state() { return state_; }

  const base::Time& session_restore_start() { return session_restore_start_; }

  
  
  bool ShouldBlockTabLoading();

 private:
  friend class MergeSessionLoadPageTest;

  
  enum {
    SESSION_RESTORE_UNDEFINED = 0,
    SESSION_RESTORE_SUCCESS = 1,
    SESSION_RESTORE_TOKEN_FETCH_FAILED = 2,
    SESSION_RESTORE_NO_REFRESH_TOKEN_FAILED = 3,
    SESSION_RESTORE_OAUTHLOGIN_FAILED = 4,
    SESSION_RESTORE_MERGE_SESSION_FAILED = 5,
    SESSION_RESTORE_COUNT = SESSION_RESTORE_MERGE_SESSION_FAILED,
  };

  
  
  
  enum PostMergeVerificationOutcome {
    POST_MERGE_UNDEFINED  = 0,
    POST_MERGE_SUCCESS = 1,
    POST_MERGE_NO_ACCOUNTS = 2,
    POST_MERGE_MISSING_PRIMARY_ACCOUNT = 3,
    POST_MERGE_PRIMARY_NOT_FIRST_ACCOUNT = 4,
    POST_MERGE_VERIFICATION_FAILED = 5,
    POST_MERGE_CONNECTION_FAILED = 6,
    POST_MERGE_COUNT = 7,
  };

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnRefreshTokenResponse(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnGetUserEmailResponse(const std::string& user_email) OVERRIDE;
  virtual void OnOAuthError() OVERRIDE;
  virtual void OnNetworkError(int response_code) OVERRIDE;

  
  virtual void OnSessionMergeSuccess() OVERRIDE;
  virtual void OnSessionMergeFailure(bool connection_error) OVERRIDE;
  virtual void OnListAccountsSuccess(const std::string& data) OVERRIDE;
  virtual void OnListAccountsFailure(bool connection_error) OVERRIDE;

  
  virtual void OnOAuth2TokensAvailable(
      const GaiaAuthConsumer::ClientOAuthResult& oauth2_tokens) OVERRIDE;
  virtual void OnOAuth2TokensFetchFailed() OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;

  
  
  void CompleteAuthentication();

  
  ProfileOAuth2TokenService* GetTokenService();

  
  
  
  
  void StoreOAuth2Token();

  
  void GetAccountIdOfRefreshToken(const std::string& refresh_token);

  
  
  void FetchOAuth2Tokens();

  
  void ReportOAuth2TokensLoaded();

  
  void RestoreSessionCookies();

  
  
  bool RetryOnError(const GoogleServiceAuthError& error);

  
  void SetSessionRestoreState(SessionRestoreState state);

  
  void SetSessionRestoreStartForTesting(const base::Time& time);

  
  static void RecordPostMergeOutcome(PostMergeVerificationOutcome outcome);

  
  
  Profile* user_profile_;
  scoped_refptr<net::URLRequestContextGetter> auth_request_context_;
  SessionRestoreStrategy restore_strategy_;
  SessionRestoreState state_;

  scoped_ptr<OAuth2TokenFetcher> oauth2_token_fetcher_;
  scoped_ptr<OAuth2LoginVerifier> login_verifier_;
  scoped_ptr<gaia::GaiaOAuthClient> account_id_fetcher_;

  
  std::string refresh_token_;

  
  std::string auth_code_;

  
  base::Time session_restore_start_;

  
  
  
  
  ObserverList<Observer, false> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(OAuth2LoginManager);
};

}  

#endif  
