// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_PROFILE_OAUTH2_TOKEN_SERVICE_REQUEST_H_
#define CHROME_BROWSER_SIGNIN_PROFILE_OAUTH2_TOKEN_SERVICE_REQUEST_H_

#include <set>
#include <string>

#include "base/threading/non_thread_safe.h"
#include "google_apis/gaia/oauth2_token_service.h"

class Profile;

class ProfileOAuth2TokenServiceRequest : public OAuth2TokenService::Request,
                                         public base::NonThreadSafe {
 public:
  
  
  static ProfileOAuth2TokenServiceRequest* CreateAndStart(
      Profile* profile,
      const std::string& account_id,
      const OAuth2TokenService::ScopeSet& scopes,
      OAuth2TokenService::Consumer* consumer);

  virtual ~ProfileOAuth2TokenServiceRequest();

  
  virtual std::string GetAccountId() const OVERRIDE;

 private:
  class Core;
  friend class Core;

  ProfileOAuth2TokenServiceRequest(Profile* profile,
                                   const std::string& account_id,
                                   const OAuth2TokenService::ScopeSet& scopes,
                                   OAuth2TokenService::Consumer* consumer);

  OAuth2TokenService::Consumer* const consumer_;
  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(ProfileOAuth2TokenServiceRequest);
};

#endif  
