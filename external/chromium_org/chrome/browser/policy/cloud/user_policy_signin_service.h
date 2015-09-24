// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_USER_POLICY_SIGNIN_SERVICE_H_
#define CHROME_BROWSER_POLICY_CLOUD_USER_POLICY_SIGNIN_SERVICE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/policy/cloud/user_policy_signin_service_base.h"
#include "chrome/browser/signin/profile_oauth2_token_service.h"

class Profile;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class CloudPolicyClientRegistrationHelper;

class UserPolicySigninService : public UserPolicySigninServiceBase,
                                public OAuth2TokenService::Observer {
 public:
  
  UserPolicySigninService(
      Profile* profile,
      PrefService* local_state,
      DeviceManagementService* device_management_service,
      scoped_refptr<net::URLRequestContextGetter> system_request_context,
      ProfileOAuth2TokenService* oauth2_token_service);
  virtual ~UserPolicySigninService();

  
  
  
  
  
  void RegisterForPolicy(const std::string& username,
                         const std::string& oauth2_login_token,
                         const PolicyRegistrationCallback& callback);

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;

  
  virtual void OnInitializationCompleted(CloudPolicyService* service) OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

 protected:
  
  virtual void InitializeUserCloudPolicyManager(
      const std::string& username,
      scoped_ptr<CloudPolicyClient> client) OVERRIDE;

  virtual void PrepareForUserCloudPolicyManagerShutdown() OVERRIDE;
  virtual void ShutdownUserCloudPolicyManager() OVERRIDE;

 private:
  
  
  
  
  void RegisterCloudPolicyService();

  
  void OnRegistrationComplete();

  
  
  void ProhibitSignoutIfNeeded();

  
  void CallPolicyRegistrationCallback(scoped_ptr<CloudPolicyClient> client,
                                      PolicyRegistrationCallback callback);

  scoped_ptr<CloudPolicyClientRegistrationHelper> registration_helper_;

  
  
  ProfileOAuth2TokenService* oauth2_token_service_;

  DISALLOW_COPY_AND_ASSIGN(UserPolicySigninService);
};

}  

#endif  
