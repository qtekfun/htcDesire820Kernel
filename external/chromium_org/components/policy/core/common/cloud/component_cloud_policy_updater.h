// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_COMPONENT_CLOUD_POLICY_UPDATER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_COMPONENT_CLOUD_POLICY_UPDATER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/cloud/external_policy_data_updater.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace enterprise_management {
class PolicyFetchResponse;
}

namespace policy {

class ComponentCloudPolicyStore;
class ExternalPolicyDataFetcher;

class POLICY_EXPORT ComponentCloudPolicyUpdater {
 public:
  
  
  
  ComponentCloudPolicyUpdater(
      scoped_refptr<base::SequencedTaskRunner> task_runner,
      scoped_ptr<ExternalPolicyDataFetcher> external_policy_data_fetcher,
      ComponentCloudPolicyStore* store);
  ~ComponentCloudPolicyUpdater();

  
  
  
  
  void UpdateExternalPolicy(
      scoped_ptr<enterprise_management::PolicyFetchResponse> response);

  
  void CancelUpdate(const PolicyNamespace& ns);

 private:
  std::string NamespaceToKey(const PolicyNamespace& ns);

  ComponentCloudPolicyStore* store_;
  ExternalPolicyDataUpdater external_policy_data_updater_;

  DISALLOW_COPY_AND_ASSIGN(ComponentCloudPolicyUpdater);
};

}  

#endif  
