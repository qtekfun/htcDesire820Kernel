// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_TOKEN_FORWARDER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_TOKEN_FORWARDER_H_

#include "base/basictypes.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/policy/core/common/cloud/cloud_policy_service.h"
#include "google_apis/gaia/oauth2_token_service.h"

class ProfileOAuth2TokenService;

namespace policy {

class UserCloudPolicyManagerChromeOS;

class UserCloudPolicyTokenForwarder : public BrowserContextKeyedService,
                                      public OAuth2TokenService::Observer,
                                      public OAuth2TokenService::Consumer,
                                      public CloudPolicyService::Observer {
 public:
  
  
  
  UserCloudPolicyTokenForwarder(UserCloudPolicyManagerChromeOS* manager,
                                ProfileOAuth2TokenService* token_service);
  virtual ~UserCloudPolicyTokenForwarder();

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;

  
  virtual void OnGetTokenSuccess(const OAuth2TokenService::Request* request,
                                 const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const OAuth2TokenService::Request* request,
                                 const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnInitializationCompleted(CloudPolicyService* service) OVERRIDE;

 private:
  void Initialize();

  void RequestAccessToken();

  UserCloudPolicyManagerChromeOS* manager_;
  ProfileOAuth2TokenService* token_service_;
  scoped_ptr<OAuth2TokenService::Request> request_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudPolicyTokenForwarder);
};

}  

#endif  
