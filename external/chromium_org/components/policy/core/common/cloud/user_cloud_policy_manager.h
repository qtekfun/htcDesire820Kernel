// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_USER_CLOUD_POLICY_MANAGER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_USER_CLOUD_POLICY_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/cloud/cloud_policy_manager.h"
#include "components/policy/policy_export.h"

class PrefService;

namespace base {
class SequencedTaskRunner;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class CloudExternalDataManager;
class DeviceManagementService;
class UserCloudPolicyStore;

class POLICY_EXPORT UserCloudPolicyManager : public CloudPolicyManager {
 public:
  
  
  
  
  
  UserCloudPolicyManager(
      scoped_ptr<UserCloudPolicyStore> store,
      const base::FilePath& component_policy_cache_path,
      scoped_ptr<CloudExternalDataManager> external_data_manager,
      const scoped_refptr<base::SequencedTaskRunner>& task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& file_task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& io_task_runner);
  virtual ~UserCloudPolicyManager();

  
  virtual void Shutdown() OVERRIDE;

  void SetSigninUsername(const std::string& username);

  
  
  
  virtual void Connect(
      PrefService* local_state,
      scoped_refptr<net::URLRequestContextGetter> request_context,
      scoped_ptr<CloudPolicyClient> client);

  
  
  
  
  void DisconnectAndRemovePolicy();

  
  
  virtual bool IsClientRegistered() const;

  
  
  
  static scoped_ptr<CloudPolicyClient> CreateCloudPolicyClient(
      DeviceManagementService* device_management_service,
      scoped_refptr<net::URLRequestContextGetter> request_context);

 private:
  
  
  scoped_ptr<UserCloudPolicyStore> store_;

  
  base::FilePath component_policy_cache_path_;

  
  scoped_ptr<CloudExternalDataManager> external_data_manager_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudPolicyManager);
};

}  

#endif  
