// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_AUTH_H_
#define CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_AUTH_H_

#include <string>

#include "base/values.h"
#include "chrome/service/cloud_print/cloud_print_url_fetcher.h"
#include "google_apis/gaia/gaia_oauth_client.h"
#include "url/gurl.h"

namespace cloud_print {

class CloudPrintAuth
    : public base::RefCountedThreadSafe<CloudPrintAuth>,
      public CloudPrintURLFetcherDelegate,
      public gaia::GaiaOAuthClient::Delegate {
 public:
  class Client {
   public:
    virtual void OnAuthenticationComplete(
        const std::string& access_token,
        const std::string& robot_oauth_refresh_token,
        const std::string& robot_email,
        const std::string& user_email) = 0;
    virtual void OnInvalidCredentials() = 0;
   protected:
     virtual ~Client() {}
  };

  CloudPrintAuth(Client* client,
                 const GURL& cloud_print_server_url,
                 const gaia::OAuthClientInfo& oauth_client_info,
                 const std::string& proxy_id);

  
  
  
  
  
  void AuthenticateWithToken(const std::string& cloud_print_token);
  void AuthenticateWithRobotToken(const std::string& robot_oauth_refresh_token,
                                  const std::string& robot_email);
  void AuthenticateWithRobotAuthCode(const std::string& robot_oauth_auth_code,
                                     const std::string& robot_email);

  void RefreshAccessToken();

  
  virtual void OnGetTokensResponse(const std::string& refresh_token,
                                   const std::string& access_token,
                                   int expires_in_seconds) OVERRIDE;
  virtual void OnRefreshTokenResponse(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnOAuthError() OVERRIDE;
  virtual void OnNetworkError(int response_code) OVERRIDE;

  
  virtual CloudPrintURLFetcher::ResponseAction HandleJSONData(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded) OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction OnRequestAuthError() OVERRIDE;
  virtual std::string GetAuthHeader() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<CloudPrintAuth>;
  virtual ~CloudPrintAuth();

  Client* client_;
  gaia::OAuthClientInfo oauth_client_info_;
  scoped_ptr<gaia::GaiaOAuthClient> oauth_client_;

  
  scoped_refptr<CloudPrintURLFetcher> request_;

  GURL cloud_print_server_url_;
  
  
  const std::string& proxy_id_;
  
  std::string refresh_token_;
  
  
  
  std::string user_email_;
  
  std::string robot_email_;
  
  
  std::string client_login_token_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintAuth);
};

}  

#endif  

