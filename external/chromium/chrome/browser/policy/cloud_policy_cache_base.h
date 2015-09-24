// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_POLICY_CACHE_BASE_H_
#define CHROME_BROWSER_POLICY_CLOUD_POLICY_CACHE_BASE_H_
#pragma once

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "base/time.h"
#include "chrome/browser/policy/cloud_policy_subsystem.h"
#include "chrome/browser/policy/configuration_policy_provider.h"
#include "chrome/browser/policy/policy_map.h"
#include "chrome/browser/policy/proto/device_management_backend.pb.h"

namespace policy {

class PolicyMap;
class PolicyNotifier;

namespace em = enterprise_management;

class CloudPolicyCacheBase : public base::NonThreadSafe {
 public:
  
  enum PolicyLevel {
    
    POLICY_LEVEL_MANDATORY,
    
    POLICY_LEVEL_RECOMMENDED,
  };

  CloudPolicyCacheBase();
  virtual ~CloudPolicyCacheBase();

  void set_policy_notifier(PolicyNotifier* notifier) {
    notifier_ = notifier;
  }

  
  virtual void Load() = 0;

  
  virtual void SetPolicy(const em::PolicyFetchResponse& policy) = 0;

  ConfigurationPolicyProvider* GetManagedPolicyProvider();
  ConfigurationPolicyProvider* GetRecommendedPolicyProvider();

  virtual void SetUnmanaged() = 0;
  bool is_unmanaged() const {
    return is_unmanaged_;
  }

  
  base::Time last_policy_refresh_time() const {
    return last_policy_refresh_time_;
  }

  
  
  
  bool GetPublicKeyVersion(int* version);

 protected:
  
  struct PublicKeyVersion {
    int version;
    bool valid;
  };

  
  
  
  
  
  
  bool SetPolicyInternal(const em::PolicyFetchResponse& policy,
                         base::Time* timestamp,
                         bool check_for_timestamp_validity);

  void SetUnmanagedInternal(const base::Time& timestamp);

  
  
  virtual bool DecodePolicyData(const em::PolicyData& policy_data,
                                PolicyMap* mandatory,
                                PolicyMap* recommended) = 0;

  
  
  bool DecodePolicyResponse(const em::PolicyFetchResponse& policy_response,
                            PolicyMap* mandatory,
                            PolicyMap* recommended,
                            base::Time* timestamp,
                            PublicKeyVersion* public_key_version);

  void InformNotifier(CloudPolicySubsystem::PolicySubsystemState state,
                      CloudPolicySubsystem::ErrorDetails error_details);

  
  bool initialization_complete() {
    return initialization_complete_;
  }

  void set_last_policy_refresh_time(base::Time timestamp) {
    last_policy_refresh_time_ = timestamp;
  }

 private:
  class CloudPolicyProvider;

  friend class DevicePolicyCacheTest;
  friend class UserPolicyCacheTest;

  
  PolicyMap mandatory_policy_;
  PolicyMap recommended_policy_;

  
  scoped_ptr<ConfigurationPolicyProvider> managed_policy_provider_;
  scoped_ptr<ConfigurationPolicyProvider> recommended_policy_provider_;

  PolicyNotifier* notifier_;

  
  
  base::Time last_policy_refresh_time_;

  
  
  
  bool initialization_complete_;

  
  bool is_unmanaged_;

  
  PublicKeyVersion public_key_version_;

  
  ObserverList<ConfigurationPolicyProvider::Observer, true> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyCacheBase);
};

}  

#endif  
