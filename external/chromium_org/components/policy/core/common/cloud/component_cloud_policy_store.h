// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_COMPONENT_CLOUD_POLICY_STORE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_COMPONENT_CLOUD_POLICY_STORE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "components/policy/core/common/cloud/resource_cache.h"
#include "components/policy/core/common/policy_bundle.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/policy_export.h"

namespace enterprise_management {
class ExternalPolicyData;
class PolicyData;
class PolicyFetchResponse;
}

namespace policy {

class POLICY_EXPORT ComponentCloudPolicyStore : public base::NonThreadSafe {
 public:
  class POLICY_EXPORT Delegate {
   public:
    virtual ~Delegate();

    
    
    virtual void OnComponentCloudPolicyStoreUpdated() = 0;
  };

  
  ComponentCloudPolicyStore(Delegate* delegate,
                            ResourceCache* cache);
  ~ComponentCloudPolicyStore();

  
  
  static bool SupportsDomain(PolicyDomain domain);

  
  
  
  static bool GetPolicyType(PolicyDomain domain, std::string* policy_type);

  
  
  
  static bool GetPolicyDomain(const std::string& policy_type,
                              PolicyDomain* domain);

  
  const PolicyBundle& policy() const { return policy_bundle_; }

  
  
  const std::string& GetCachedHash(const PolicyNamespace& ns) const;

  
  
  
  void SetCredentials(const std::string& username,
                      const std::string& dm_token);

  
  
  
  void Load();

  
  
  
  
  
  
  
  bool Store(const PolicyNamespace& ns,
             const std::string& serialized_policy_proto,
             const std::string& secure_hash,
             const std::string& data);

  
  void Delete(const PolicyNamespace& ns);

  
  
  void Purge(PolicyDomain domain,
             const ResourceCache::SubkeyFilter& filter);

  
  void Clear();

  
  
  
  
  
  bool ValidatePolicy(
      scoped_ptr<enterprise_management::PolicyFetchResponse> proto,
      PolicyNamespace* ns,
      enterprise_management::ExternalPolicyData* payload);

 private:
  
  
  bool ValidateProto(
      scoped_ptr<enterprise_management::PolicyFetchResponse> proto,
      const std::string& policy_type,
      const std::string& settings_entity_id,
      enterprise_management::ExternalPolicyData* payload,
      enterprise_management::PolicyData* policy_data);

  
  
  
  bool ValidateData(const std::string& data,
                    const std::string& secure_hash,
                    PolicyMap* policy);

  
  
  bool ParsePolicy(const std::string& data, PolicyMap* policy);

  Delegate* delegate_;
  ResourceCache* cache_;
  std::string username_;
  std::string dm_token_;

  PolicyBundle policy_bundle_;
  std::map<PolicyNamespace, std::string> cached_hashes_;

  DISALLOW_COPY_AND_ASSIGN(ComponentCloudPolicyStore);
};

}  

#endif  
