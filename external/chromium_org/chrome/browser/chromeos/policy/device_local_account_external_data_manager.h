// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_LOCAL_ACCOUNT_EXTERNAL_DATA_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_LOCAL_ACCOUNT_EXTERNAL_DATA_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/policy/cloud_external_data_manager_base.h"
#include "components/policy/core/common/policy_details.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class ResourceCache;

class DeviceLocalAccountExternalDataManager
    : public CloudExternalDataManagerBase,
      public base::RefCounted<DeviceLocalAccountExternalDataManager> {
 private:
  friend class DeviceLocalAccountExternalDataService;
  friend class base::RefCounted<DeviceLocalAccountExternalDataManager>;

  
  
  
  
  
  
  
  
  
  DeviceLocalAccountExternalDataManager(
      const std::string& account_id,
      const GetChromePolicyDetailsCallback& get_policy_details,
      scoped_refptr<base::SequencedTaskRunner> backend_task_runner,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner,
      ResourceCache* resource_cache);
  virtual ~DeviceLocalAccountExternalDataManager();

  
  virtual void OnPolicyStoreLoaded() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DeviceLocalAccountExternalDataManager);
};

}  

#endif  
