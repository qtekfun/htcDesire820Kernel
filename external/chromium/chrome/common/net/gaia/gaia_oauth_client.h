// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_GAIA_GAIA_OAUTH_CLIENT_H_
#define CHROME_COMMON_NET_GAIA_GAIA_OAUTH_CLIENT_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "base/message_loop_proxy.h"

namespace net {
class URLRequestContextGetter;
}

namespace gaia {

static const char kGaiaOAuth2Url[] =
    "https://accounts.google.com/o/oauth2/token";

struct OAuthClientInfo {
  std::string client_id;
  std::string client_secret;
};

class GaiaOAuthClient {
 public:
  class Delegate {
   public:
    virtual ~Delegate() { }

    
    virtual void OnGetTokensResponse(const std::string& refresh_token,
                                     const std::string& access_token,
                                     int expires_in_seconds) = 0;
    
    virtual void OnRefreshTokenResponse(const std::string& access_token,
                                        int expires_in_seconds) = 0;
    
    virtual void OnOAuthError() = 0;
    
    
    
    virtual void OnNetworkError(int response_code) = 0;
  };
  GaiaOAuthClient(const std::string& gaia_url,
                  net::URLRequestContextGetter* context_getter);
  ~GaiaOAuthClient();

  
  
  
  
  void GetTokensFromAuthCode(const OAuthClientInfo& oauth_client_info,
                             const std::string& auth_code,
                             int max_retries,
                             Delegate* delegate);
  void RefreshToken(const OAuthClientInfo& oauth_client_info,
                    const std::string& refresh_token,
                    int max_retries,
                    Delegate* delegate);

 private:
  
  class Core;
  scoped_refptr<Core> core_;
  DISALLOW_COPY_AND_ASSIGN(GaiaOAuthClient);
};
}

#endif  
