// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PUSH_MESSAGING_OBFUSCATED_GAIA_ID_FETCHER_H_
#define CHROME_BROWSER_EXTENSIONS_API_PUSH_MESSAGING_OBFUSCATED_GAIA_ID_FETCHER_H_

#include <string>

#include "google_apis/gaia/oauth2_api_call_flow.h"

class GoogleServiceAuthError;

namespace content {
class URLFetcher;
}

namespace net {
class URLRequestContextGetter;
}

namespace extensions {

class ObfuscatedGaiaIdFetcher : public OAuth2ApiCallFlow {
 public:
  
  
  class Delegate {
   public:
    virtual void OnObfuscatedGaiaIdFetchSuccess(
        const std::string& obfuscated_id) {}
    virtual void OnObfuscatedGaiaIdFetchFailure(
        const GoogleServiceAuthError& error) {}
    virtual ~Delegate() {}
  };

  
  

  ObfuscatedGaiaIdFetcher(net::URLRequestContextGetter* context,
                          Delegate* delegate,
                          const std::string& refresh_token);
  virtual ~ObfuscatedGaiaIdFetcher();

  static std::vector<std::string> GetScopes();

 protected:
  
  virtual GURL CreateApiCallUrl() OVERRIDE;
  virtual std::string CreateApiCallBody() OVERRIDE;
  virtual void ProcessApiCallSuccess(
      const net::URLFetcher* source) OVERRIDE;
  virtual void ProcessApiCallFailure(
      const net::URLFetcher* source) OVERRIDE;
  virtual void ProcessNewAccessToken(const std::string& access_token) OVERRIDE;
  virtual void ProcessMintAccessTokenFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedGaiaIdFetcherTest, SetUp);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedGaiaIdFetcherTest, ParseResponse);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedGaiaIdFetcherTest, ProcessApiCallSuccess);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedGaiaIdFetcherTest, ProcessApiCallFailure);

  void ReportSuccess(const std::string& obfuscated_id);
  void ReportFailure(const GoogleServiceAuthError& error);

  
  static bool ParseResponse(
      const std::string& data, std::string* obfuscated_id);

  
  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(ObfuscatedGaiaIdFetcher);
};

}  

#endif  
