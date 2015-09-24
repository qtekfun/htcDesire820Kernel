// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PROVIDER_H_
#define CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PROVIDER_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "chrome/browser/policy/configuration_policy_store_interface.h"
#include "policy/configuration_policy_type.h"

namespace policy {

class PolicyMap;

class ConfigurationPolicyProvider {
 public:
  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnUpdatePolicy() = 0;
    virtual void OnProviderGoingAway() = 0;
  };

  
  
  struct PolicyDefinitionList {
    struct Entry {
      ConfigurationPolicyType policy_type;
      Value::ValueType value_type;
      const char* name;
    };

    const Entry* begin;
    const Entry* end;
  };

  explicit ConfigurationPolicyProvider(const PolicyDefinitionList* policy_list);

  virtual ~ConfigurationPolicyProvider();

  
  
  
  
  
  
  virtual bool Provide(ConfigurationPolicyStoreInterface* store) = 0;

  
  
  
  virtual bool IsInitializationComplete() const;

 protected:
  
  void ApplyPolicyValueTree(const DictionaryValue* policies,
                             ConfigurationPolicyStoreInterface* store);

  
  
  void ApplyPolicyMap(const PolicyMap* policies,
                      ConfigurationPolicyStoreInterface* store);

  const PolicyDefinitionList* policy_definition_list() const {
    return policy_definition_list_;
  }

 private:
  friend class ConfigurationPolicyObserverRegistrar;

  
  
  friend class UserPolicyCache;

  virtual void AddObserver(ConfigurationPolicyProvider::Observer* observer) = 0;
  virtual void RemoveObserver(
      ConfigurationPolicyProvider::Observer* observer) = 0;

  
  const ConfigurationPolicyProvider::PolicyDefinitionList*
      policy_definition_list_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyProvider);
};

class ConfigurationPolicyObserverRegistrar
    : ConfigurationPolicyProvider::Observer {
 public:
  ConfigurationPolicyObserverRegistrar();
  ~ConfigurationPolicyObserverRegistrar();
  void Init(ConfigurationPolicyProvider* provider,
            ConfigurationPolicyProvider::Observer* observer);

  
  virtual void OnUpdatePolicy();
  virtual void OnProviderGoingAway();

 private:
  ConfigurationPolicyProvider* provider_;
  ConfigurationPolicyProvider::Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyObserverRegistrar);
};

}  

#endif  
