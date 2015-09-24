// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_OAUTH2_ACCESS_TOKEN_CONSUMER_H_
#define GOOGLE_APIS_GAIA_OAUTH2_ACCESS_TOKEN_CONSUMER_H_

#include <string>

class GoogleServiceAuthError;

namespace base {
class Time;
}

class OAuth2AccessTokenConsumer {
 public:
  
  
  
  virtual void OnGetTokenSuccess(const std::string& access_token,
                                 const base::Time& expiration_time) {}
  virtual void OnGetTokenFailure(const GoogleServiceAuthError& error) {}

 protected:
  virtual ~OAuth2AccessTokenConsumer() {}
};

#endif  
