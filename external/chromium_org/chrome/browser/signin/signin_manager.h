// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_H_

#if defined(OS_CHROMEOS)
#include "chrome/browser/signin/signin_manager_base.h"

#else

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/signin/signin_internals_util.h"
#include "chrome/browser/signin/signin_manager_base.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "net/cookies/canonical_cookie.h"

class CookieSettings;
class GaiaAuthFetcher;
class ProfileIOData;
class PrefService;
class SigninAccountIdHelper;
class SigninGlobalError;
class SigninManagerDelegate;

class SigninManager : public SigninManagerBase,
                      public GaiaAuthConsumer,
                      public content::NotificationObserver {
 public:
  
  
  
  
  typedef base::Callback<void(const std::string&)> OAuthTokenFetchedCallback;

  
  
  static bool IsWebBasedSigninFlowURL(const GURL& url);

  
  
  
  
  
  static const char* kChromeSigninEffectiveSite;

  explicit SigninManager(scoped_ptr<SigninManagerDelegate> delegate);
  virtual ~SigninManager();

  
  static bool IsUsernameAllowedByPolicy(const std::string& username,
                                        const std::string& policy);

  
  
  
  
  
  
  
  
  
  
  virtual void StartSignInWithCredentials(
      const std::string& session_index,
      const std::string& username,
      const std::string& password,
      const OAuthTokenFetchedCallback& oauth_fetched_callback);

  
  
  
  virtual void StartSignInWithOAuthCode(
      const std::string& username,
      const std::string& password,
      const std::string& oauth_code,
      const OAuthTokenFetchedCallback& callback);

  
  
  
  virtual void CopyCredentialsFrom(const SigninManager& source);

  
  
  virtual void SignOut();

  
  
  
  virtual void Initialize(Profile* profile, PrefService* local_state) OVERRIDE;
  virtual void Shutdown() OVERRIDE;

  
  virtual void CompletePendingSignin();

  
  
  void OnExternalSigninCompleted(const std::string& username);

  
  virtual bool AuthInProgress() const OVERRIDE;

  virtual bool IsSigninAllowed() const OVERRIDE;

  
  
  virtual bool IsAllowedUsername(const std::string& username) const;

  
  
  const std::string& GetUsernameForAuthInProgress() const;

  
  
  void OnGetUserInfoKeyNotFound(const std::string& key);

  
  
  static void DisableOneClickSignIn(Profile* profile);

  
  virtual void OnClientLoginSuccess(const ClientLoginResult& result) OVERRIDE;
  virtual void OnClientLoginFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnClientOAuthSuccess(const ClientOAuthResult& result) OVERRIDE;
  virtual void OnClientOAuthFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnGetUserInfoSuccess(const UserInfoMap& data) OVERRIDE;
  virtual void OnGetUserInfoFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void ProhibitSignout(bool prohibit_signout);

  
  
  bool IsSignoutProhibited() const;

  
  
  
  static bool IsSigninAllowedOnIOThread(ProfileIOData* io_data);

  
  
  
  
  
  void SetSigninProcess(int process_id);
  void ClearSigninProcess();
  bool IsSigninProcess(int process_id) const;
  bool HasSigninProcess() const;

 protected:
  
  bool prohibit_signout_;

 private:
  enum SigninType {
    SIGNIN_TYPE_NONE,
    SIGNIN_TYPE_WITH_CREDENTIALS,
    SIGNIN_TYPE_WITH_OAUTH_CODE
  };

  std::string SigninTypeToString(SigninType type);
  friend class FakeSigninManager;
  FRIEND_TEST_ALL_PREFIXES(SigninManagerTest, ClearTransientSigninData);
  FRIEND_TEST_ALL_PREFIXES(SigninManagerTest, ProvideSecondFactorSuccess);
  FRIEND_TEST_ALL_PREFIXES(SigninManagerTest, ProvideSecondFactorFailure);

  
  void InitTokenService();

  
  
  
  
  
  bool PrepareForSignin(SigninType type,
                        const std::string& username,
                        const std::string& password);

  
  
  void VerifyGaiaCookiesBeforeSignIn(const std::string& session_index);

  
  
  void OnGaiaCookiesFetched(
      const std::string session_index, const net::CookieList& cookie_list);

  
  
  void OnSignedIn(const std::string& username);

  
  
  
  void ClearTransientSigninData();

  
  
  
  void HandleAuthError(const GoogleServiceAuthError& error,
                       bool clear_transient_data);

  void OnSigninAllowedPrefChanged();
  void OnGoogleServicesUsernamePatternChanged();

  
  std::string possibly_invalid_username_;
  std::string password_;  
  bool had_two_factor_error_;

  
  
  ClientLoginResult last_result_;

  
  scoped_ptr<GaiaAuthFetcher> client_login_;

  
  content::NotificationRegistrar registrar_;

  
  scoped_ptr<GaiaAuthFetcher> revoke_token_fetcher_;

  
  scoped_ptr<SigninAccountIdHelper> account_id_helper_;

  
  
  
  SigninType type_;

  
  
  
  ClientOAuthResult temp_oauth_login_tokens_;

  base::WeakPtrFactory<SigninManager> weak_pointer_factory_;

  
  
  int signin_process_id_;

  
  
  OAuthTokenFetchedCallback oauth_token_fetched_callback_;

  scoped_ptr<SigninManagerDelegate> delegate_;

  
  
  PrefChangeRegistrar local_state_pref_registrar_;

  
  BooleanPrefMember signin_allowed_;

  DISALLOW_COPY_AND_ASSIGN(SigninManager);
};

#endif  

#endif  
