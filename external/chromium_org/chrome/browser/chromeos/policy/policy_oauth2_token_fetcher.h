// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_POLICY_OAUTH2_TOKEN_FETCHER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_POLICY_OAUTH2_TOKEN_FETCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/oauth2_access_token_consumer.h"

class GaiaAuthFetcher;
class OAuth2AccessTokenFetcher;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class PolicyOAuth2TokenFetcher
    : public base::SupportsWeakPtr<PolicyOAuth2TokenFetcher>,
      public GaiaAuthConsumer,
      public OAuth2AccessTokenConsumer {
 public:
  typedef base::Callback<void(const std::string&,
                              const GoogleServiceAuthError&)> TokenCallback;

  
  
  PolicyOAuth2TokenFetcher(net::URLRequestContextGetter* auth_context_getter,
                           net::URLRequestContextGetter* system_context_getter,
                           const TokenCallback& callback);

  virtual ~PolicyOAuth2TokenFetcher();

  
  void Start();

  
  const GaiaAuthConsumer::ClientOAuthResult& oauth2_tokens() const {
    return oauth2_tokens_;
  }

  
  
  bool has_oauth2_tokens() const {
    return !oauth2_tokens_.refresh_token.empty();
  }

  
  
  bool failed() const {
    return failed_;
  }

  const std::string& oauth2_refresh_token() const {
    return oauth2_refresh_token_;
  }
  const std::string& oauth2_access_token() const {
    return oauth2_access_token_;
  }

 private:
  
  virtual void OnClientOAuthSuccess(
      const GaiaAuthConsumer::ClientOAuthResult& oauth_tokens) OVERRIDE;
  virtual void OnClientOAuthFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnGetTokenSuccess(const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const GoogleServiceAuthError& error) OVERRIDE;

  
  void StartFetchingRefreshToken();

  
  void StartFetchingAccessToken();

  
  
  
  
  void RetryOnError(const GoogleServiceAuthError& error,
                    const base::Closure& task);

  
  void ForwardPolicyToken(const std::string& token,
                          const GoogleServiceAuthError& error);

  scoped_refptr<net::URLRequestContextGetter> auth_context_getter_;
  scoped_refptr<net::URLRequestContextGetter> system_context_getter_;
  scoped_ptr<GaiaAuthFetcher> refresh_token_fetcher_;
  scoped_ptr<OAuth2AccessTokenFetcher> access_token_fetcher_;
  GaiaAuthConsumer::ClientOAuthResult oauth2_tokens_;

  
  
  std::string oauth2_refresh_token_;

  
  std::string oauth2_access_token_;

  
  int retry_count_;

  
  bool failed_;

  
  TokenCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(PolicyOAuth2TokenFetcher);
};

}  

#endif  
