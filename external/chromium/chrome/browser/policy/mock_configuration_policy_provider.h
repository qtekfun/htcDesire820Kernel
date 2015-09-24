// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_MOCK_CONFIGURATION_POLICY_PROVIDER_H_
#define CHROME_BROWSER_POLICY_MOCK_CONFIGURATION_POLICY_PROVIDER_H_
#pragma once

#include <map>
#include <utility>

#include "chrome/browser/policy/configuration_policy_provider.h"
#include "chrome/browser/policy/policy_map.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace policy {

class MockConfigurationPolicyProvider : public ConfigurationPolicyProvider {
 public:
  MockConfigurationPolicyProvider();
  virtual ~MockConfigurationPolicyProvider();

  void AddPolicy(ConfigurationPolicyType policy, Value* value);
  void RemovePolicy(ConfigurationPolicyType policy);

  void SetInitializationComplete(bool initialization_complete);

  
  virtual bool Provide(ConfigurationPolicyStoreInterface* store);
  virtual bool IsInitializationComplete() const;

 private:
  
  virtual void AddObserver(ConfigurationPolicyProvider::Observer* observer) {}
  virtual void RemoveObserver(
      ConfigurationPolicyProvider::Observer* observer) {}

  PolicyMap policy_map_;
  bool initialization_complete_;
};

}  

#endif  
