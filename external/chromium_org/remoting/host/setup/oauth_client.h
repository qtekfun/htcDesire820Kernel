// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_OAUTH_CLIENT
#define REMOTING_HOST_SETUP_OAUTH_CLIENT

#include <queue>
#include <string>

#include "base/callback.h"
#include "google_apis/gaia/gaia_oauth_client.h"
#include "remoting/base/url_request_context.h"

namespace remoting {

class OAuthClient : public gaia::GaiaOAuthClient::Delegate {
 public:
  
  
  
  typedef base::Callback<void(
      const std::string& user_email,
      const std::string& refresh_token)> CompletionCallback;

  OAuthClient(
      scoped_refptr<net::URLRequestContextGetter> url_request_context_getter);

  virtual ~OAuthClient();

  
  
  
  
  
  
  void GetCredentialsFromAuthCode(
      const gaia::OAuthClientInfo& oauth_client_info,
      const std::string& auth_code,
      CompletionCallback on_done);

  
  virtual void OnGetTokensResponse(const std::string& refresh_token,
                                 const std::string& access_token,
                                 int expires_in_seconds) OVERRIDE;
  virtual void OnRefreshTokenResponse(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnGetUserEmailResponse(const std::string& user_email) OVERRIDE;

  virtual void OnOAuthError() OVERRIDE;
  virtual void OnNetworkError(int response_code) OVERRIDE;

 private:
  struct Request {
    Request(const gaia::OAuthClientInfo& oauth_client_info,
            const std::string& auth_code,
            CompletionCallback on_done);
    virtual ~Request();
    gaia::OAuthClientInfo oauth_client_info;
    std::string auth_code;
    CompletionCallback on_done;
  };

  void SendResponse(const std::string& user_email,
                    const std::string& refresh_token);

  std::queue<Request> pending_requests_;
  gaia::GaiaOAuthClient gaia_oauth_client_;
  std::string refresh_token_;
  CompletionCallback on_done_;

  DISALLOW_COPY_AND_ASSIGN(OAuthClient);
};

}  

#endif  
