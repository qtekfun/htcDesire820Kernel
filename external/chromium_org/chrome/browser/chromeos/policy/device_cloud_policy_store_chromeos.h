// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_CLOUD_POLICY_STORE_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_CLOUD_POLICY_STORE_CHROMEOS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/policy/device_cloud_policy_validator.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"

namespace base {
class SequencedTaskRunner;
}

namespace enterprise_management {
class PolicyFetchResponse;
}

namespace policy {

class EnterpriseInstallAttributes;

class DeviceCloudPolicyStoreChromeOS
    : public CloudPolicyStore,
      public chromeos::DeviceSettingsService::Observer {
 public:
  DeviceCloudPolicyStoreChromeOS(
      chromeos::DeviceSettingsService* device_settings_service,
      EnterpriseInstallAttributes* install_attributes,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);
  virtual ~DeviceCloudPolicyStoreChromeOS();

  
  virtual void Store(
      const enterprise_management::PolicyFetchResponse& policy) OVERRIDE;
  virtual void Load() OVERRIDE;

  
  
  
  
  
  void InstallInitialPolicy(
      const enterprise_management::PolicyFetchResponse& policy);

  
  virtual void OwnershipStatusChanged() OVERRIDE;
  virtual void DeviceSettingsUpdated() OVERRIDE;

 private:
  
  
  scoped_ptr<DeviceCloudPolicyValidator> CreateValidator(
      const enterprise_management::PolicyFetchResponse& policy);

  
  
  void OnPolicyToStoreValidated(DeviceCloudPolicyValidator* validator);

  
  void OnPolicyStored();

  
  void UpdateFromService();

  chromeos::DeviceSettingsService* device_settings_service_;
  EnterpriseInstallAttributes* install_attributes_;

  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  base::WeakPtrFactory<DeviceCloudPolicyStoreChromeOS> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DeviceCloudPolicyStoreChromeOS);
};

}  

#endif  
