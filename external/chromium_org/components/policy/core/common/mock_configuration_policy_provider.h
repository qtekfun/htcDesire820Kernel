// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_MOCK_CONFIGURATION_POLICY_PROVIDER_H_
#define COMPONENTS_POLICY_CORE_COMMON_MOCK_CONFIGURATION_POLICY_PROVIDER_H_

#include "base/basictypes.h"
#include "components/policy/core/common/configuration_policy_provider.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/core/common/schema_registry.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace policy {

class MockConfigurationPolicyProvider : public ConfigurationPolicyProvider {
 public:
  MockConfigurationPolicyProvider();
  virtual ~MockConfigurationPolicyProvider();

  MOCK_CONST_METHOD1(IsInitializationComplete, bool(PolicyDomain domain));
  MOCK_METHOD0(RefreshPolicies, void());

  
  using ConfigurationPolicyProvider::UpdatePolicy;

  
  
  void UpdateChromePolicy(const PolicyMap& policy);

  
  
  using ConfigurationPolicyProvider::Init;
  void Init() {
    ConfigurationPolicyProvider::Init(&registry_);
  }

  
  
  void SetAutoRefresh();

 private:
  void RefreshWithSamePolicies();

  SchemaRegistry registry_;

  DISALLOW_COPY_AND_ASSIGN(MockConfigurationPolicyProvider);
};

class MockConfigurationPolicyObserver
    : public ConfigurationPolicyProvider::Observer {
 public:
  MockConfigurationPolicyObserver();
  virtual ~MockConfigurationPolicyObserver();

  MOCK_METHOD1(OnUpdatePolicy, void(ConfigurationPolicyProvider*));
};

}  

#endif  
