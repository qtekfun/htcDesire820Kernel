// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_MANAGER_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_MANAGER_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/core/common/cloud/cloud_policy_manager.h"
#include "components/policy/core/common/cloud/cloud_policy_service.h"

class GoogleServiceAuthError;
class PrefService;

namespace base {
class SequencedTaskRunner;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class CloudExternalDataManager;
class DeviceManagementService;
class PolicyOAuth2TokenFetcher;
class WildcardLoginChecker;

class UserCloudPolicyManagerChromeOS
    : public CloudPolicyManager,
      public CloudPolicyClient::Observer,
      public CloudPolicyService::Observer,
      public BrowserContextKeyedService {
 public:
  
  
  
  
  
  
  
  UserCloudPolicyManagerChromeOS(
      scoped_ptr<CloudPolicyStore> store,
      scoped_ptr<CloudExternalDataManager> external_data_manager,
      const base::FilePath& component_policy_cache_path,
      bool wait_for_policy_fetch,
      base::TimeDelta initial_policy_fetch_timeout,
      const scoped_refptr<base::SequencedTaskRunner>& task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& file_task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& io_task_runner);
  virtual ~UserCloudPolicyManagerChromeOS();

  
  
  void Connect(
      PrefService* local_state,
      DeviceManagementService* device_management_service,
      scoped_refptr<net::URLRequestContextGetter> system_request_context,
      UserAffiliation user_affiliation);

  
  
  
  
  
  
  
  
  
  void OnAccessTokenAvailable(const std::string& access_token);

  
  bool IsClientRegistered() const;

  
  
  void EnableWildcardLoginCheck(const std::string& username);

  
  virtual void Shutdown() OVERRIDE;
  virtual bool IsInitializationComplete(PolicyDomain domain) const OVERRIDE;

  
  virtual void OnInitializationCompleted(CloudPolicyService* service) OVERRIDE;

  
  virtual void OnPolicyFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRegistrationStateChanged(CloudPolicyClient* client) OVERRIDE;
  virtual void OnClientError(CloudPolicyClient* client) OVERRIDE;

  
  virtual void OnComponentCloudPolicyUpdated() OVERRIDE;

 private:
  
  
  void FetchPolicyOAuthTokenUsingSigninProfile();

  
  
  void OnOAuth2PolicyTokenFetched(const std::string& policy_token,
                                  const GoogleServiceAuthError& error);

  
  
  
  void OnInitialPolicyFetchComplete(bool success);

  
  
  
  void CancelWaitForPolicyFetch();

  void StartRefreshSchedulerIfReady();

  
  scoped_ptr<CloudPolicyStore> store_;

  
  scoped_ptr<CloudExternalDataManager> external_data_manager_;

  
  std::string wildcard_username_;

  
  base::FilePath component_policy_cache_path_;

  
  
  bool wait_for_policy_fetch_;

  
  
  base::Timer policy_fetch_timeout_;

  
  PrefService* local_state_;

  
  
  scoped_ptr<PolicyOAuth2TokenFetcher> token_fetcher_;

  
  scoped_ptr<WildcardLoginChecker> wildcard_login_checker_;

  
  
  std::string access_token_;

  
  base::Time time_init_started_;
  base::Time time_init_completed_;
  base::Time time_token_available_;
  base::Time time_client_registered_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudPolicyManagerChromeOS);
};

}  

#endif  
