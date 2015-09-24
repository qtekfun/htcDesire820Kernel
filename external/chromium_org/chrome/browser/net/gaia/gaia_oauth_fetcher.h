// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_GAIA_GAIA_OAUTH_FETCHER_H_
#define CHROME_BROWSER_NET_GAIA_GAIA_OAUTH_FETCHER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class GaiaOAuthConsumer;

namespace net {
class URLFetcher;
class URLRequestContextGetter;
class URLRequestStatus;
typedef std::vector<std::string> ResponseCookies;
}

class GaiaOAuthFetcher : public net::URLFetcherDelegate {
 public:
  
  typedef enum {
    OAUTH1_LOGIN,
    OAUTH1_REQUEST_TOKEN,
    OAUTH1_ALL_ACCESS_TOKEN,
    OAUTH2_SERVICE_ACCESS_TOKEN,
    USER_INFO,
    OAUTH2_REVOKE_TOKEN,
  } RequestType;

  GaiaOAuthFetcher(GaiaOAuthConsumer* consumer,
                   net::URLRequestContextGetter* getter,
                   const std::string& service_scope);

  virtual ~GaiaOAuthFetcher();

  
  
  
  void SetAutoFetchLimit(RequestType limit) { auto_fetch_limit_ = limit; }

  
  
  void StartGetOAuthTokenRequest();

  
  void StartOAuthLogin(const char* source,
                       const char* service,
                       const std::string& oauth1_access_token,
                       const std::string& oauth1_access_token_secret);

  
  
  
  virtual void StartOAuthGetAccessToken(
      const std::string& oauth1_request_token);

  
  
  
  
  
  
  
  
  
  
  
  virtual void StartOAuthWrapBridge(
      const std::string& oauth1_access_token,
      const std::string& oauth1_access_token_secret,
      const std::string& wrap_token_duration,
      const std::string& service_scope);

  
  
  
  virtual void StartUserInfo(const std::string& oauth2_access_token);

  
  
  virtual void StartOAuthRevokeAccessToken(const std::string& token,
                                           const std::string& secret);

  
  
  virtual void StartOAuthRevokeWrapToken(const std::string& token);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  virtual bool HasPendingFetch() const;

  
  virtual void CancelRequest();

 protected:
  
  RequestType request_type_;

 private:
  
  virtual void OnGetOAuthTokenUrlFetched(const net::ResponseCookies& cookies,
                                         const net::URLRequestStatus& status,
                                         int response_code);

  
  virtual void OnOAuthLoginFetched(const std::string& data,
                                   const net::URLRequestStatus& status,
                                   int response_code);

  
  virtual void OnOAuthGetAccessTokenFetched(const std::string& data,
                                            const net::URLRequestStatus& status,
                                            int response_code);

  
  virtual void OnOAuthWrapBridgeFetched(const std::string& data,
                                        const net::URLRequestStatus& status,
                                        int response_code);

  
  virtual void OnOAuthRevokeTokenFetched(const std::string& data,
                                         const net::URLRequestStatus& status,
                                         int response_code);

  
  virtual void OnUserInfoFetched(const std::string& data,
                                 const net::URLRequestStatus& status,
                                 int response_code);

  
  static void ParseOAuthLoginResponse(const std::string& data,
                                      std::string* sid,
                                      std::string* lsid,
                                      std::string* auth);

  
  static void ParseOAuthGetAccessTokenResponse(const std::string& data,
                                               std::string* token,
                                               std::string* secret);

  
  static void ParseOAuthWrapBridgeResponse(const std::string& data,
                                           std::string* token,
                                           std::string* expires_in);

  
  static void ParseUserInfoResponse(const std::string& data,
                                    std::string* email);

  
  static GoogleServiceAuthError GenerateAuthError(
      const std::string& data,
      const net::URLRequestStatus& status,
      int response_code);

  
  static GURL MakeGetOAuthTokenUrl(const std::string& oauth1_login_scope,
                                   const std::string& product_name);

  
  static std::string MakeOAuthGetAccessTokenBody(
      const std::string& oauth1_request_token);

  
  static std::string MakeOAuthLoginBody(
      const char* source,
      const char* service,
      const std::string& oauth1_access_token,
      const std::string& oauth1_access_token_secret);

  
  static std::string MakeOAuthWrapBridgeBody(
      const std::string& oauth1_access_token,
      const std::string& oauth1_access_token_secret,
      const std::string& wrap_token_duration,
      const std::string& oauth2_service_scope);

  
  static net::URLFetcher* CreateGaiaFetcher(
      net::URLRequestContextGetter* getter,
      const GURL& gaia_gurl_,
      const std::string& body,
      const std::string& headers,
      bool send_cookies,
      net::URLFetcherDelegate* delegate);

  bool ShouldAutoFetch(RequestType fetch_step);

  
  GaiaOAuthConsumer* const consumer_;
  net::URLRequestContextGetter* const getter_;

  
  scoped_ptr<net::URLFetcher> fetcher_;
  std::string request_body_;
  std::string request_headers_;
  std::string service_scope_;
  bool fetch_pending_;
  RequestType auto_fetch_limit_;

  DISALLOW_COPY_AND_ASSIGN(GaiaOAuthFetcher);
};

#endif  
