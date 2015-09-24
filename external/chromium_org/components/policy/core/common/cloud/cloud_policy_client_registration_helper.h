// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_CLIENT_REGISTRATION_HELPER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_CLIENT_REGISTRATION_HELPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/user_info_fetcher.h"
#include "components/policy/policy_export.h"
#include "policy/proto/device_management_backend.pb.h"

class OAuth2TokenService;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class POLICY_EXPORT CloudPolicyClientRegistrationHelper
    : public UserInfoFetcher::Delegate,
      public CloudPolicyClient::Observer {
 public:
  
  
  
  
  CloudPolicyClientRegistrationHelper(
      CloudPolicyClient* client,
      bool should_force_load_policy,
      enterprise_management::DeviceRegisterRequest::Type registration_type);
  virtual ~CloudPolicyClientRegistrationHelper();

  
  
  
  
  void StartRegistration(
      OAuth2TokenService* token_service,
      const std::string& account_id,
      const base::Closure& callback);

#if !defined(OS_ANDROID)
  
  
  
  void StartRegistrationWithLoginToken(const std::string& login_refresh_token,
                                       const base::Closure& callback);
#endif

 private:
  class TokenServiceHelper;
#if !defined(OS_ANDROID)
  class LoginTokenHelper;
#endif

  void OnTokenFetched(const std::string& oauth_access_token);

  
  virtual void OnGetUserInfoSuccess(
      const base::DictionaryValue* response) OVERRIDE;
  virtual void OnGetUserInfoFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnPolicyFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRegistrationStateChanged(CloudPolicyClient* client) OVERRIDE;
  virtual void OnClientError(CloudPolicyClient* client) OVERRIDE;

  
  void RequestCompleted();

  
  
  
  
  scoped_ptr<TokenServiceHelper> token_service_helper_;

#if !defined(OS_ANDROID)
  
  
  
  scoped_ptr<LoginTokenHelper> login_token_helper_;
#endif

  
  
  scoped_ptr<UserInfoFetcher> user_info_fetcher_;

  
  
  std::string oauth_access_token_;

  net::URLRequestContextGetter* context_;
  CloudPolicyClient* client_;
  bool should_force_load_policy_;
  enterprise_management::DeviceRegisterRequest::Type registration_type_;
  base::Closure callback_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyClientRegistrationHelper);
};

}  

#endif  
