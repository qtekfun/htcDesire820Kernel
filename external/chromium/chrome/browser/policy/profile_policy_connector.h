// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_PROFILE_POLICY_CONNECTOR_H_
#define CHROME_BROWSER_POLICY_PROFILE_POLICY_CONNECTOR_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/policy/configuration_policy_provider.h"

class Profile;

namespace policy {

class CloudPolicySubsystem;
class UserPolicyIdentityStrategy;

class ProfilePolicyConnector {
 public:
  explicit ProfilePolicyConnector(Profile* profile);
  ~ProfilePolicyConnector();

  
  
  void Initialize();

  
  
  void Shutdown();

  ConfigurationPolicyProvider* GetManagedCloudProvider();
  ConfigurationPolicyProvider* GetRecommendedCloudProvider();

 private:
  Profile* profile_;

  scoped_ptr<UserPolicyIdentityStrategy> identity_strategy_;
  scoped_ptr<CloudPolicySubsystem> cloud_policy_subsystem_;

  scoped_ptr<ConfigurationPolicyProvider> managed_cloud_provider_;
  scoped_ptr<ConfigurationPolicyProvider> recommended_cloud_provider_;

  DISALLOW_COPY_AND_ASSIGN(ProfilePolicyConnector);
};

class MergingPolicyProvider: public ConfigurationPolicyProvider,
                             public ConfigurationPolicyProvider::Observer {
 public:
  MergingPolicyProvider(ConfigurationPolicyProvider* browser_policy_provider,
                        ConfigurationPolicyProvider* profile_policy_provider);
  virtual ~MergingPolicyProvider();

  
  virtual bool Provide(ConfigurationPolicyStoreInterface* store) OVERRIDE;
  virtual void AddObserver(
      ConfigurationPolicyProvider::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(
      ConfigurationPolicyProvider::Observer* observer) OVERRIDE;

  
  virtual void OnUpdatePolicy() OVERRIDE;
  virtual void OnProviderGoingAway() OVERRIDE;

 private:
  ConfigurationPolicyProvider* browser_policy_provider_;
  ConfigurationPolicyProvider* profile_policy_provider_;
  scoped_ptr<ConfigurationPolicyObserverRegistrar> browser_registrar_;
  scoped_ptr<ConfigurationPolicyObserverRegistrar> profile_registrar_;
  ObserverList<ConfigurationPolicyProvider::Observer, true> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(MergingPolicyProvider);
};


}  

#endif  
