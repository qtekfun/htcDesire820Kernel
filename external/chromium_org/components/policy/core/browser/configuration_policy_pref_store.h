// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_BROWSER_CONFIGURATION_POLICY_PREF_STORE_H_
#define COMPONENTS_POLICY_CORE_BROWSER_CONFIGURATION_POLICY_PREF_STORE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_store.h"
#include "base/values.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/core/common/policy_service.h"
#include "components/policy/core/common/policy_types.h"
#include "components/policy/policy_export.h"

class PrefValueMap;

namespace policy {

class ConfigurationPolicyHandlerList;

class POLICY_EXPORT ConfigurationPolicyPrefStore
    : public PrefStore,
      public PolicyService::Observer {
 public:
  
  
  ConfigurationPolicyPrefStore(
      PolicyService* service,
      const ConfigurationPolicyHandlerList* handler_list,
      PolicyLevel level);

  
  virtual void AddObserver(PrefStore::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(PrefStore::Observer* observer) OVERRIDE;
  virtual bool HasObservers() const OVERRIDE;
  virtual bool IsInitializationComplete() const OVERRIDE;
  virtual bool GetValue(const std::string& key,
                        const Value** result) const OVERRIDE;

  
  virtual void OnPolicyUpdated(const PolicyNamespace& ns,
                               const PolicyMap& previous,
                               const PolicyMap& current) OVERRIDE;
  virtual void OnPolicyServiceInitialized(PolicyDomain domain) OVERRIDE;

 private:
  virtual ~ConfigurationPolicyPrefStore();

  
  
  void Refresh();

  
  
  PrefValueMap* CreatePreferencesFromPolicies();

  
  PolicyService* policy_service_;

  
  
  const ConfigurationPolicyHandlerList* handler_list_;

  
  PolicyLevel level_;

  
  scoped_ptr<PrefValueMap> prefs_;

  ObserverList<PrefStore::Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyPrefStore);
};

}  

#endif  
