// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_LOGIN_PROFILE_POLICY_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_LOGIN_PROFILE_POLICY_PROVIDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/core/common/configuration_policy_provider.h"
#include "components/policy/core/common/policy_service.h"

namespace policy {

class LoginProfilePolicyProvider : public ConfigurationPolicyProvider,
                                   public PolicyService::Observer {
 public:
  explicit LoginProfilePolicyProvider(PolicyService* device_policy_service);
  virtual ~LoginProfilePolicyProvider();

  
  virtual void Init(SchemaRegistry* registry) OVERRIDE;
  virtual void Shutdown() OVERRIDE;
  virtual void RefreshPolicies() OVERRIDE;

  
  virtual void OnPolicyUpdated(const PolicyNamespace& ns,
                               const PolicyMap& previous,
                               const PolicyMap& current) OVERRIDE;
  virtual void OnPolicyServiceInitialized(PolicyDomain domain) OVERRIDE;

  void OnDevicePolicyRefreshDone();

 private:
  void UpdateFromDevicePolicy();

  PolicyService* device_policy_service_;  

  bool waiting_for_device_policy_refresh_;

  base::WeakPtrFactory<LoginProfilePolicyProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(LoginProfilePolicyProvider);
};

}  

#endif  
