// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_GAIA_AUTH_FETCHER_H_
#define GOOGLE_APIS_GAIA_GAIA_AUTH_FETCHER_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"


class GaiaAuthFetcherTest;

namespace net {
class URLFetcher;
class URLRequestContextGetter;
class URLRequestStatus;
}

class GaiaAuthFetcher : public net::URLFetcherDelegate {
 public:
  enum HostedAccountsSetting {
    HostedAccountsAllowed,
    HostedAccountsNotAllowed
  };

  
  
  static const char kSecondFactor[];

  
  
  GaiaAuthFetcher(GaiaAuthConsumer* consumer,
                  const std::string& source,
                  net::URLRequestContextGetter* getter);
  virtual ~GaiaAuthFetcher();

  
  
  
  
  
  
  
  
  
  
  void StartClientLogin(const std::string& username,
                        const std::string& password,
                        const char* const service,
                        const std::string& login_token,
                        const std::string& login_captcha,
                        HostedAccountsSetting allow_hosted_accounts);

  
  
  
  
  
  void StartIssueAuthToken(const std::string& sid,
                           const std::string& lsid,
                           const char* const service);

  
  
  
  
  
  void StartTokenAuth(const std::string& uber_token,
                      const char* const service);

  
  
  
  
  
  void StartIssueAuthTokenForOAuth2(const std::string& oauth2_access_token,
                                    const char* const service);

  
  
  
  
  
  void StartLsoForOAuthLoginTokenExchange(const std::string& auth_token);

  
  
  
  
  void StartRevokeOAuth2Token(const std::string& auth_token);

  
  
  
  
  
  
  
  void StartCookieForOAuthLoginTokenExchange(const std::string& session_index);

  
  
  
  
  
  void StartAuthCodeForOAuth2TokenExchange(const std::string& auth_code);

  
  
  
  
  void StartGetUserInfo(const std::string& lsid);

  
  
  
  
  
  
  
  
  
  void StartMergeSession(const std::string& uber_token);

  
  
  
  
  
  
  void StartTokenFetchForUberAuthExchange(const std::string& access_token);

  
  
  
  
  
  
  
  
  void StartOAuthLogin(const std::string& access_token,
                       const std::string& service);

  
  void StartListAccounts();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  bool HasPendingFetch();

  
  void CancelRequest();

  
  
  
  static GoogleServiceAuthError GenerateOAuthLoginError(
      const std::string& data,
      const net::URLRequestStatus& status);

 private:
  
  static const char kCookiePersistence[];
  static const char kAccountTypeHostedOrGoogle[];
  static const char kAccountTypeGoogle[];

  
  static const char kClientLoginFormat[];
  
  static const char kClientLoginCaptchaFormat[];
  
  static const char kIssueAuthTokenFormat[];
  
  static const char kClientLoginToOAuth2BodyFormat[];
  
  static const char kOAuth2CodeToTokenPairBodyFormat[];
  
  static const char kOAuth2RevokeTokenBodyFormat[];
  
  static const char kGetUserInfoFormat[];
  
  static const char kMergeSessionFormat[];
  
  static const char kUberAuthTokenURLFormat[];
  
  static const char kOAuthLoginFormat[];

  
  static const char kAccountDeletedError[];
  static const char kAccountDeletedErrorCode[];
  static const char kAccountDisabledError[];
  static const char kAccountDisabledErrorCode[];
  static const char kBadAuthenticationError[];
  static const char kBadAuthenticationErrorCode[];
  static const char kCaptchaError[];
  static const char kCaptchaErrorCode[];
  static const char kServiceUnavailableError[];
  static const char kServiceUnavailableErrorCode[];
  static const char kErrorParam[];
  static const char kErrorUrlParam[];
  static const char kCaptchaUrlParam[];
  static const char kCaptchaTokenParam[];

  
  static const char kNeedsAdditional[];
  static const char kCaptcha[];
  static const char kTwoFactor[];

  
  static const char kAuthHeaderFormat[];
  static const char kOAuthHeaderFormat[];
  static const char kOAuth2BearerHeaderFormat[];
  static const char kClientLoginToOAuth2CookiePartSecure[];
  static const char kClientLoginToOAuth2CookiePartHttpOnly[];
  static const char kClientLoginToOAuth2CookiePartCodePrefix[];
  static const int kClientLoginToOAuth2CookiePartCodePrefixLength;

  
  void OnClientLoginFetched(const std::string& data,
                            const net::URLRequestStatus& status,
                            int response_code);

  void OnIssueAuthTokenFetched(const std::string& data,
                               const net::URLRequestStatus& status,
                               int response_code);

  void OnClientLoginToOAuth2Fetched(const std::string& data,
                                    const net::ResponseCookies& cookies,
                                    const net::URLRequestStatus& status,
                                    int response_code);

  void OnOAuth2TokenPairFetched(const std::string& data,
                                const net::URLRequestStatus& status,
                                int response_code);

  void OnOAuth2RevokeTokenFetched(const std::string& data,
                                  const net::URLRequestStatus& status,
                                  int response_code);

  void OnListAccountsFetched(const std::string& data,
                             const net::URLRequestStatus& status,
                             int response_code);

  void OnGetUserInfoFetched(const std::string& data,
                            const net::URLRequestStatus& status,
                            int response_code);

  void OnMergeSessionFetched(const std::string& data,
                             const net::URLRequestStatus& status,
                             int response_code);

  void OnUberAuthTokenFetch(const std::string& data,
                            const net::URLRequestStatus& status,
                            int response_code);

  void OnOAuthLoginFetched(const std::string& data,
                           const net::URLRequestStatus& status,
                           int response_code);

  
  static void ParseClientLoginResponse(const std::string& data,
                                       std::string* sid,
                                       std::string* lsid,
                                       std::string* token);

  static void ParseClientLoginFailure(const std::string& data,
                                      std::string* error,
                                      std::string* error_url,
                                      std::string* captcha_url,
                                      std::string* captcha_token);

  
  static bool ParseClientLoginToOAuth2Response(
      const net::ResponseCookies& cookies,
      std::string* auth_code);

  static bool ParseClientLoginToOAuth2Cookie(const std::string& cookie,
                                             std::string* auth_code);

  
  static bool IsSecondFactorSuccess(const std::string& alleged_error);

  
  static std::string MakeClientLoginBody(
      const std::string& username,
      const std::string& password,
      const std::string& source,
      const char* const service,
      const std::string& login_token,
      const std::string& login_captcha,
      HostedAccountsSetting allow_hosted_accounts);
  
  
  static std::string MakeIssueAuthTokenBody(const std::string& sid,
                                            const std::string& lsid,
                                            const char* const service);
  
  static std::string MakeGetAuthCodeBody();
  
  static std::string MakeGetTokenPairBody(const std::string& auth_code);
  
  std::string MakeRevokeTokenBody(const std::string& auth_token);
  
  
  static std::string MakeGetUserInfoBody(const std::string& lsid);

  
  static std::string MakeMergeSessionBody(const std::string& auth_token,
                                       const std::string& continue_url,
                                       const std::string& source);

  static std::string MakeGetAuthCodeHeader(const std::string& auth_token);

  static std::string MakeOAuthLoginBody(const std::string& service,
                                        const std::string& source);

  
  
  
  
  
  
  static net::URLFetcher* CreateGaiaFetcher(
      net::URLRequestContextGetter* getter,
      const std::string& body,
      const std::string& headers,
      const GURL& gaia_gurl,
      int load_flags,
      net::URLFetcherDelegate* delegate);

  
  
  
  static GoogleServiceAuthError GenerateAuthError(
      const std::string& data,
      const net::URLRequestStatus& status);

  
  GaiaAuthConsumer* const consumer_;
  net::URLRequestContextGetter* const getter_;
  std::string source_;
  const GURL client_login_gurl_;
  const GURL issue_auth_token_gurl_;
  const GURL oauth2_token_gurl_;
  const GURL oauth2_revoke_gurl_;
  const GURL get_user_info_gurl_;
  const GURL merge_session_gurl_;
  const GURL uberauth_token_gurl_;
  const GURL oauth_login_gurl_;
  const GURL list_accounts_gurl_;

  
  scoped_ptr<net::URLFetcher> fetcher_;
  GURL client_login_to_oauth2_gurl_;
  std::string request_body_;
  std::string requested_service_; 
  bool fetch_pending_;

  friend class GaiaAuthFetcherTest;
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, CaptchaParse);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, AccountDeletedError);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, AccountDisabledError);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, BadAuthenticationError);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, IncomprehensibleError);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, ServiceUnavailableError);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, CheckNormalErrorCode);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, CheckTwoFactorResponse);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, LoginNetFailure);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest,
      ParseClientLoginToOAuth2Response);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, ParseOAuth2TokenPairResponse);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, ClientOAuthSuccess);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, ClientOAuthWithQuote);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, ClientOAuthChallengeSuccess);
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthFetcherTest, ClientOAuthChallengeQuote);

  DISALLOW_COPY_AND_ASSIGN(GaiaAuthFetcher);
};

#endif  
