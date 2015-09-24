// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_GAIA_AUTH_CONSUMER_H_
#define GOOGLE_APIS_GAIA_GAIA_AUTH_CONSUMER_H_

#include <map>
#include <string>
#include <vector>

class GoogleServiceAuthError;

namespace net {
typedef std::vector<std::string> ResponseCookies;
}

typedef std::map<std::string, std::string> UserInfoMap;

class GaiaAuthConsumer {
 public:
  struct ClientLoginResult {
    ClientLoginResult();
    ClientLoginResult(const std::string& new_sid,
                      const std::string& new_lsid,
                      const std::string& new_token,
                      const std::string& new_data);
    ~ClientLoginResult();

    bool operator==(const ClientLoginResult &b) const;

    std::string sid;
    std::string lsid;
    std::string token;
    
    std::string data;  
    bool two_factor;  
  };

  struct ClientOAuthResult {
    ClientOAuthResult();
    ClientOAuthResult(const std::string& new_refresh_token,
                      const std::string& new_access_token,
                      int new_expires_in_secs);
    ~ClientOAuthResult();

    bool operator==(const ClientOAuthResult &b) const;

    
    std::string refresh_token;

    
    
    std::string access_token;

    
    int expires_in_secs;
  };

  virtual ~GaiaAuthConsumer() {}

  virtual void OnClientLoginSuccess(const ClientLoginResult& result) {}
  virtual void OnClientLoginFailure(const GoogleServiceAuthError& error) {}

  virtual void OnIssueAuthTokenSuccess(const std::string& service,
                                       const std::string& auth_token) {}
  virtual void OnIssueAuthTokenFailure(const std::string& service,
                                       const GoogleServiceAuthError& error) {}

  virtual void OnClientOAuthSuccess(const ClientOAuthResult& result) {}
  virtual void OnClientOAuthFailure(const GoogleServiceAuthError& error) {}

  virtual void OnOAuth2RevokeTokenCompleted() {}

  virtual void OnGetUserInfoSuccess(const UserInfoMap& data) {}
  virtual void OnGetUserInfoFailure(const GoogleServiceAuthError& error) {}

  virtual void OnUberAuthTokenSuccess(const std::string& token) {}
  virtual void OnUberAuthTokenFailure(const GoogleServiceAuthError& error) {}

  virtual void OnMergeSessionSuccess(const std::string& data) {}
  virtual void OnMergeSessionFailure(const GoogleServiceAuthError& error) {}

  virtual void OnListAccountsSuccess(const std::string& data) {}
  virtual void OnListAccountsFailure(const GoogleServiceAuthError& error) {}
};

#endif  
