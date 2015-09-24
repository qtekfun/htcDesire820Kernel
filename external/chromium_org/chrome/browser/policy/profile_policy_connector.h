// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_PROFILE_POLICY_CONNECTOR_H_
#define CHROME_BROWSER_POLICY_PROFILE_POLICY_CONNECTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace chromeos {
class User;
}

namespace policy {

class CloudPolicyManager;
class ConfigurationPolicyProvider;
class PolicyService;
class SchemaRegistry;

class ProfilePolicyConnector : public BrowserContextKeyedService {
 public:
  ProfilePolicyConnector();
  virtual ~ProfilePolicyConnector();

  
  void Init(bool force_immediate_load,
#if defined(OS_CHROMEOS)
            const chromeos::User* user,
#endif
            SchemaRegistry* schema_registry,
            CloudPolicyManager* user_cloud_policy_manager);

  void InitForTesting(scoped_ptr<PolicyService> service);

  
  virtual void Shutdown() OVERRIDE;

  
  PolicyService* policy_service() const { return policy_service_.get(); }

 private:
#if defined(ENABLE_CONFIGURATION_POLICY)
#if defined(OS_CHROMEOS)
  void InitializeDeviceLocalAccountPolicyProvider(
      const std::string& username,
      SchemaRegistry* schema_registry);

  
  
  
  
  
  bool is_primary_user_;

  scoped_ptr<ConfigurationPolicyProvider> special_user_policy_provider_;
#endif  

  scoped_ptr<ConfigurationPolicyProvider> forwarding_policy_provider_;
#endif  

  scoped_ptr<PolicyService> policy_service_;

  DISALLOW_COPY_AND_ASSIGN(ProfilePolicyConnector);
};

}  

#endif  
