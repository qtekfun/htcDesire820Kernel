// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_GAIA_GAIA_AUTH_CONSUMER_H_
#define CHROME_COMMON_NET_GAIA_GAIA_AUTH_CONSUMER_H_
#pragma once

#include <string>

class GoogleServiceAuthError;

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

  virtual ~GaiaAuthConsumer() {}

  virtual void OnClientLoginSuccess(const ClientLoginResult& result) {}
  virtual void OnClientLoginFailure(const GoogleServiceAuthError& error) {}

  virtual void OnIssueAuthTokenSuccess(const std::string& service,
                                       const std::string& auth_token) {}
  virtual void OnIssueAuthTokenFailure(const std::string& service,
                                       const GoogleServiceAuthError& error) {}

  virtual void OnGetUserInfoSuccess(const std::string& key,
                                    const std::string& value) {}
  virtual void OnGetUserInfoKeyNotFound(const std::string& key) {}
  virtual void OnGetUserInfoFailure(const GoogleServiceAuthError& error) {}
};

#endif  
