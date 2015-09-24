// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_GAIA_OAUTH_CLIENT_H_
#define GOOGLE_APIS_GAIA_GAIA_OAUTH_CLIENT_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/values.h"

namespace net {
class URLRequestContextGetter;
}

namespace gaia {

struct OAuthClientInfo {
  std::string client_id;
  std::string client_secret;
  std::string redirect_uri;
};

class GaiaOAuthClient {
 public:
  const static int kUrlFetcherId;

  class Delegate {
   public:
    
    virtual void OnGetTokensResponse(const std::string& refresh_token,
                                     const std::string& access_token,
                                     int expires_in_seconds) {}
    
    virtual void OnRefreshTokenResponse(const std::string& access_token,
                                        int expires_in_seconds) {}
    
    virtual void OnGetUserEmailResponse(const std::string& user_email) {}
    
    virtual void OnGetUserIdResponse(const std::string& user_id) {}
    
    virtual void OnGetTokenInfoResponse(
        scoped_ptr<DictionaryValue> token_info) {}
    
    virtual void OnOAuthError() = 0;
    
    
    
    virtual void OnNetworkError(int response_code) = 0;

   protected:
    virtual ~Delegate() {}
  };

  GaiaOAuthClient(net::URLRequestContextGetter* context_getter);
  ~GaiaOAuthClient();

  
  
  
  

  
  
  
  void GetTokensFromAuthCode(const OAuthClientInfo& oauth_client_info,
                             const std::string& auth_code,
                             int max_retries,
                             Delegate* delegate);

  
  
  
  
  
  void RefreshToken(const OAuthClientInfo& oauth_client_info,
                    const std::string& refresh_token,
                    const std::vector<std::string>& scopes,
                    int max_retries,
                    Delegate* delegate);

  
  
  
  
  void GetUserEmail(const std::string& oauth_access_token,
                    int max_retries,
                    Delegate* delegate);

  
  
  
  
  void GetUserId(const std::string& oauth_access_token,
                 int max_retries,
                 Delegate* delegate);

  
  
  
  
  
  
  
  void GetTokenInfo(const std::string& oauth_access_token,
                    int max_retries,
                    Delegate* delegate);

 private:
  
  class Core;
  scoped_refptr<Core> core_;
  DISALLOW_COPY_AND_ASSIGN(GaiaOAuthClient);
};
}

#endif  
