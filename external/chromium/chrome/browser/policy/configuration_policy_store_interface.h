// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_STORE_INTERFACE_H_
#define CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_STORE_INTERFACE_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "policy/configuration_policy_type.h"

class Value;

namespace policy {

enum PolicyProxyModeType {
  
  kPolicyNoProxyServerMode = 0,
  
  kPolicyAutoDetectProxyServerMode = 1,
  
  kPolicyManuallyConfiguredProxyServerMode = 2,
  
  kPolicyUseSystemProxyServerMode = 3,

  MODE_COUNT
};

class ConfigurationPolicyStoreInterface {
 public:
  virtual ~ConfigurationPolicyStoreInterface() {}

  
  
  
  virtual void Apply(ConfigurationPolicyType policy, Value* value) = 0;

 protected:
  ConfigurationPolicyStoreInterface() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyStoreInterface);
};

class ObservingPolicyStoreInterface: public ConfigurationPolicyStoreInterface {
 public:
  explicit ObservingPolicyStoreInterface(
      ConfigurationPolicyStoreInterface* next)
      : next_(next),
        proxy_policy_applied_(false) {}

  
  virtual void Apply(ConfigurationPolicyType policy, Value* value) OVERRIDE;

  bool IsProxyPolicyApplied() const {
    return proxy_policy_applied_;
  }

 private:
  ConfigurationPolicyStoreInterface* next_;
  bool proxy_policy_applied_;

  DISALLOW_COPY_AND_ASSIGN(ObservingPolicyStoreInterface);
};

class FilteringPolicyStoreInterface: public ConfigurationPolicyStoreInterface {
 public:
  FilteringPolicyStoreInterface(ConfigurationPolicyStoreInterface* next,
                                bool apply_proxy_policies)
      : next_(next),
        apply_proxy_policies_(apply_proxy_policies) {}

  
  virtual void Apply(ConfigurationPolicyType policy, Value* value) OVERRIDE;

 private:
  ConfigurationPolicyStoreInterface* next_;
  bool apply_proxy_policies_;

  DISALLOW_COPY_AND_ASSIGN(FilteringPolicyStoreInterface);
};

}  

#endif  
