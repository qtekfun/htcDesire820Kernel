// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_POLICY_HEADER_SERVICE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_POLICY_HEADER_SERVICE_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class PolicyHeaderIOHelper;

class POLICY_EXPORT PolicyHeaderService : public CloudPolicyStore::Observer {
 public:
  
  
  
  PolicyHeaderService(const std::string& server_url,
                      CloudPolicyStore* user_policy_store,
                      CloudPolicyStore* device_policy_store);
  virtual ~PolicyHeaderService();

  
  
  
  
  
  scoped_ptr<PolicyHeaderIOHelper> CreatePolicyHeaderIOHelper(
      scoped_refptr<base::SequencedTaskRunner> task_runner);

  
  virtual void OnStoreLoaded(CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(CloudPolicyStore* store) OVERRIDE;

 private:
  
  std::string CreateHeaderValue();

  
  std::vector<PolicyHeaderIOHelper*> helpers_;

  
  std::string server_url_;

  
  CloudPolicyStore* user_policy_store_;
  CloudPolicyStore* device_policy_store_;

  DISALLOW_COPY_AND_ASSIGN(PolicyHeaderService);
};

}  

#endif  
