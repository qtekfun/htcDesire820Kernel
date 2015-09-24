// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_EXTERNAL_DATA_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_EXTERNAL_DATA_MANAGER_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/policy/cloud_external_data_manager_base.h"
#include "components/policy/core/common/policy_details.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class CloudPolicyStore;
class ResourceCache;

class UserCloudExternalDataManager : public CloudExternalDataManagerBase {
 public:
  
  
  
  
  
  
  UserCloudExternalDataManager(
      const GetChromePolicyDetailsCallback& get_policy_details,
      scoped_refptr<base::SequencedTaskRunner> backend_task_runner,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner,
      const base::FilePath& cache_path,
      CloudPolicyStore* policy_store);
  virtual ~UserCloudExternalDataManager();

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  ResourceCache* resource_cache_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudExternalDataManager);
};

}  

#endif  
