// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_OAUTH2_API_CALL_FLOW_H_
#define GOOGLE_APIS_GAIA_OAUTH2_API_CALL_FLOW_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "google_apis/gaia/oauth2_access_token_consumer.h"
#include "google_apis/gaia/oauth2_access_token_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"

class GoogleServiceAuthError;
class OAuth2MintTokenFlowTest;

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

class OAuth2ApiCallFlow
    : public net::URLFetcherDelegate,
      public OAuth2AccessTokenConsumer {
 public:
  
  
  
  OAuth2ApiCallFlow(
      net::URLRequestContextGetter* context,
      const std::string& refresh_token,
      const std::string& access_token,
      const std::vector<std::string>& scopes);

  virtual ~OAuth2ApiCallFlow();

  
  virtual void Start();

  
  virtual void OnGetTokenSuccess(const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

 protected:
  

  
  virtual GURL CreateApiCallUrl() = 0;
  virtual std::string CreateApiCallBody() = 0;

  
  
  
  virtual void ProcessApiCallSuccess(const net::URLFetcher* source) = 0;
  
  virtual void ProcessApiCallFailure(const net::URLFetcher* source) = 0;
  
  virtual void ProcessNewAccessToken(const std::string& access_token) = 0;
  virtual void ProcessMintAccessTokenFailure(
      const GoogleServiceAuthError& error) = 0;

 private:
  enum State {
    INITIAL,
    API_CALL_STARTED,
    API_CALL_DONE,
    MINT_ACCESS_TOKEN_STARTED,
    MINT_ACCESS_TOKEN_DONE,
    ERROR_STATE
  };

  friend class OAuth2ApiCallFlowTest;
  FRIEND_TEST_ALL_PREFIXES(OAuth2ApiCallFlowTest, CreateURLFetcher);

  
  
  
  virtual OAuth2AccessTokenFetcher* CreateAccessTokenFetcher();

  
  
  
  
  
  
  virtual net::URLFetcher* CreateURLFetcher();

  
  void BeginApiCall();
  void EndApiCall(const net::URLFetcher* source);
  void BeginMintAccessToken();
  void EndMintAccessToken(const GoogleServiceAuthError* error);

  net::URLRequestContextGetter* context_;
  std::string refresh_token_;
  std::string access_token_;
  std::vector<std::string> scopes_;

  State state_;
  
  bool tried_mint_access_token_;

  scoped_ptr<net::URLFetcher> url_fetcher_;
  scoped_ptr<OAuth2AccessTokenFetcher> oauth2_access_token_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(OAuth2ApiCallFlow);
};

#endif  
