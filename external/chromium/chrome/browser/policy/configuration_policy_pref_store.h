// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PREF_STORE_H_
#define CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PREF_STORE_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/values.h"
#include "chrome/browser/policy/configuration_policy_provider.h"
#include "chrome/browser/policy/configuration_policy_store_interface.h"
#include "chrome/common/pref_store.h"

class Profile;

namespace policy {

class ConfigurationPolicyPrefKeeper;

class ConfigurationPolicyPrefStore
    : public PrefStore,
      public ConfigurationPolicyProvider::Observer {
 public:
  
  
  explicit ConfigurationPolicyPrefStore(ConfigurationPolicyProvider* provider);
  virtual ~ConfigurationPolicyPrefStore();

  
  virtual void AddObserver(PrefStore::Observer* observer);
  virtual void RemoveObserver(PrefStore::Observer* observer);
  virtual bool IsInitializationComplete() const;
  virtual ReadResult GetValue(const std::string& key,
                              const Value** result) const;

  
  virtual void OnUpdatePolicy();
  virtual void OnProviderGoingAway();

  
  static ConfigurationPolicyPrefStore* CreateManagedPlatformPolicyPrefStore();

  
  static ConfigurationPolicyPrefStore* CreateManagedCloudPolicyPrefStore(
      Profile* profile);

  
  
  static ConfigurationPolicyPrefStore*
      CreateRecommendedPlatformPolicyPrefStore();

  
  static ConfigurationPolicyPrefStore* CreateRecommendedCloudPolicyPrefStore(
      Profile* profile);

  
  static const ConfigurationPolicyProvider::PolicyDefinitionList*
      GetChromePolicyDefinitionList();

 private:
  
  
  void Refresh();

  static const ConfigurationPolicyProvider::PolicyDefinitionList
      kPolicyDefinitionList;

  
  ConfigurationPolicyProvider* provider_;

  
  
  bool initialization_complete_;

  
  scoped_ptr<ConfigurationPolicyPrefKeeper> policy_keeper_;

  ObserverList<PrefStore::Observer, true> observers_;

  ConfigurationPolicyObserverRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyPrefStore);
};

}  

#endif  
