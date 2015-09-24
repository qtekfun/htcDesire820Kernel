// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_REFRESH_TOKEN_FETCHER_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_REFRESH_TOKEN_FETCHER_H_

#include <string>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

class GoogleServiceAuthError;
class OAuth2TokenService;

namespace net {
class URLRequestContextGetter;
}

class ManagedUserRefreshTokenFetcher {
 public:
  typedef base::Callback<void(const GoogleServiceAuthError& ,
                              const std::string& )>
      TokenCallback;

  static scoped_ptr<ManagedUserRefreshTokenFetcher> Create(
      OAuth2TokenService* oauth2_token_service,
      const std::string& account_id,
      net::URLRequestContextGetter* context);

  virtual ~ManagedUserRefreshTokenFetcher();

  virtual void Start(const std::string& managed_user_id,
                     const std::string& device_name,
                     const TokenCallback& callback) = 0;
};

#endif  
