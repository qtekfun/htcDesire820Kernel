// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// tokens, which will be written to the database after retrieval, as well as

#ifndef CHROME_BROWSER_NET_GAIA_TOKEN_SERVICE_H_
#define CHROME_BROWSER_NET_GAIA_TOKEN_SERVICE_H_
#pragma once

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/webdata/web_data_service.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_fetcher.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;

namespace net {
class URLRequestContextGetter;
}

class TokenService : public GaiaAuthConsumer,
                     public WebDataServiceConsumer,
                     public NotificationObserver {
 public:
   TokenService();
   virtual ~TokenService();

  
  class TokenAvailableDetails {
   public:
    TokenAvailableDetails() {}
    TokenAvailableDetails(const std::string& service,
                          const std::string& token)
        : service_(service), token_(token) {}
    const std::string& service() const { return service_; }
    const std::string& token() const { return token_; }
   private:
    std::string service_;
    std::string token_;
  };

  class TokenRequestFailedDetails {
   public:
    TokenRequestFailedDetails()
        : error_(GoogleServiceAuthError::NONE) {}
    TokenRequestFailedDetails(const std::string& service,
                              const GoogleServiceAuthError& error)
        : service_(service), error_(error) {}
    const std::string& service() const { return service_; }
    const GoogleServiceAuthError& error() const { return error_; }
   private:
    std::string service_;
    GoogleServiceAuthError error_;
  };

  
  
  
  void Initialize(const char* const source, Profile* profile);

  
  
  void UpdateCredentials(
      const GaiaAuthConsumer::ClientLoginResult& credentials);

  
  
  
  
  
  void ResetCredentialsInMemory();

  
  
  
  void LoadTokensFromDB();

  
  
  void EraseTokensFromDB();

  
  
  bool HasLsid() const;
  const std::string& GetLsid() const;
  
  bool AreCredentialsValid() const;

  
  
  
  void StartFetchingTokens();
  bool HasTokenForService(const char* const service) const;
  const std::string& GetTokenForService(const char* const service) const;

  
  void IssueAuthTokenForTest(const std::string& service,
                             const std::string& auth_token);

  
  virtual void OnIssueAuthTokenSuccess(const std::string& service,
                                       const std::string& auth_token);
  virtual void OnIssueAuthTokenFailure(const std::string& service,
                                       const GoogleServiceAuthError& error);

  
  virtual void OnWebDataServiceRequestDone(WebDataService::Handle h,
                                           const WDTypedResult* result);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:

  void FireTokenAvailableNotification(const std::string& service,
                                      const std::string& auth_token);

  void FireTokenRequestFailedNotification(const std::string& service,
                                          const GoogleServiceAuthError& error);

  void LoadTokensIntoMemory(const std::map<std::string, std::string>& in_toks,
                            std::map<std::string, std::string>* out_toks);

  void SaveAuthTokenToDB(const std::string& service,
                         const std::string& auth_token);

  
  scoped_refptr<WebDataService> web_data_service_;
  
  scoped_refptr<net::URLRequestContextGetter> getter_;
  
  WebDataService::Handle token_loading_query_;

  
  std::string source_;
  
  GaiaAuthConsumer::ClientLoginResult credentials_;

  
  static const int kNumServices = 4;
  
  static const char* kServices[kNumServices];
  
  
  scoped_ptr<GaiaAuthFetcher> fetchers_[kNumServices];
  
  std::map<std::string, std::string> token_map_;

  NotificationRegistrar registrar_;

  FRIEND_TEST_ALL_PREFIXES(TokenServiceTest, LoadTokensIntoMemoryBasic);
  FRIEND_TEST_ALL_PREFIXES(TokenServiceTest, LoadTokensIntoMemoryAdvanced);

  DISALLOW_COPY_AND_ASSIGN(TokenService);
};

#endif  
