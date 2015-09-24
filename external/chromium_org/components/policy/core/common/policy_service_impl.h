// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_SERVICE_IMPL_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_SERVICE_IMPL_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "components/policy/core/common/configuration_policy_provider.h"
#include "components/policy/core/common/policy_bundle.h"
#include "components/policy/core/common/policy_service.h"
#include "components/policy/policy_export.h"

namespace policy {

class PolicyMap;

class POLICY_EXPORT PolicyServiceImpl
    : public PolicyService,
      public ConfigurationPolicyProvider::Observer {
 public:
  typedef std::vector<ConfigurationPolicyProvider*> Providers;

  
  
  
  
  explicit PolicyServiceImpl(const Providers& providers);

  virtual ~PolicyServiceImpl();

  
  virtual void AddObserver(PolicyDomain domain,
                           PolicyService::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(PolicyDomain domain,
                              PolicyService::Observer* observer) OVERRIDE;
  virtual const PolicyMap& GetPolicies(
      const PolicyNamespace& ns) const OVERRIDE;
  virtual bool IsInitializationComplete(PolicyDomain domain) const OVERRIDE;
  virtual void RefreshPolicies(const base::Closure& callback) OVERRIDE;

 private:
  typedef ObserverList<PolicyService::Observer, true> Observers;
  typedef std::map<PolicyDomain, Observers*> ObserverMap;

  
  virtual void OnUpdatePolicy(ConfigurationPolicyProvider* provider) OVERRIDE;

  
  
  void NotifyNamespaceUpdated(const PolicyNamespace& ns,
                              const PolicyMap& previous,
                              const PolicyMap& current);

  
  
  void MergeAndTriggerUpdates();

  
  
  void CheckInitializationComplete();

  
  void CheckRefreshComplete();

  
  Providers providers_;

  
  PolicyBundle policy_bundle_;

  
  ObserverMap observers_;

  
  bool initialization_complete_[POLICY_DOMAIN_SIZE];

  
  
  std::set<ConfigurationPolicyProvider*> refresh_pending_;

  
  
  std::vector<base::Closure> refresh_callbacks_;

  
  
  base::WeakPtrFactory<PolicyServiceImpl> update_task_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PolicyServiceImpl);
};

}  

#endif  
