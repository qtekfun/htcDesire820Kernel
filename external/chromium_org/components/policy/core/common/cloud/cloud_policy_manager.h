// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_MANAGER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/core/common/cloud/cloud_policy_core.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/core/common/cloud/component_cloud_policy_service.h"
#include "components/policy/core/common/configuration_policy_provider.h"
#include "components/policy/policy_export.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class POLICY_EXPORT CloudPolicyManager
    : public ConfigurationPolicyProvider,
      public CloudPolicyStore::Observer,
      public ComponentCloudPolicyService::Delegate {
 public:
  
  
  
  
  
  CloudPolicyManager(
      const PolicyNamespaceKey& policy_ns_key,
      CloudPolicyStore* cloud_policy_store,
      const scoped_refptr<base::SequencedTaskRunner>& task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& file_task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& io_task_runner);
  virtual ~CloudPolicyManager();

  CloudPolicyCore* core() { return &core_; }
  const CloudPolicyCore* core() const { return &core_; }

  
  virtual void Shutdown() OVERRIDE;
  virtual bool IsInitializationComplete(PolicyDomain domain) const OVERRIDE;
  virtual void RefreshPolicies() OVERRIDE;

  
  virtual void OnStoreLoaded(CloudPolicyStore* cloud_policy_store) OVERRIDE;
  virtual void OnStoreError(CloudPolicyStore* cloud_policy_store) OVERRIDE;

  
  virtual void OnComponentCloudPolicyUpdated() OVERRIDE;

 protected:
  
  
  void CheckAndPublishPolicy();

  void CreateComponentCloudPolicyService(
      const base::FilePath& policy_cache_path,
      const scoped_refptr<net::URLRequestContextGetter>& request_context);

  void ClearAndDestroyComponentCloudPolicyService();

  
  CloudPolicyClient* client() { return core_.client(); }
  const CloudPolicyClient* client() const { return core_.client(); }
  CloudPolicyStore* store() { return core_.store(); }
  const CloudPolicyStore* store() const { return core_.store(); }
  CloudPolicyService* service() { return core_.service(); }
  const CloudPolicyService* service() const { return core_.service(); }
  ComponentCloudPolicyService* component_policy_service() const {
    return component_policy_service_.get();
  }

 private:
  
  void OnRefreshComplete(bool success);

  CloudPolicyCore core_;
  scoped_ptr<ComponentCloudPolicyService> component_policy_service_;

  
  
  bool waiting_for_policy_refresh_;

  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyManager);
};

}  

#endif  
