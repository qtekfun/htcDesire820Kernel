// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_CLOUD_POLICY_MANAGER_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_CLOUD_POLICY_MANAGER_CHROMEOS_H_

#include <bitset>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/policy/enrollment_status_chromeos.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/cloud_policy_manager.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"

namespace base {
class SequencedTaskRunner;
}

namespace chromeos {
namespace attestation {
class AttestationPolicyObserver;
}
}

class PrefRegistrySimple;
class PrefService;

namespace policy {

class DeviceCloudPolicyStoreChromeOS;
class DeviceManagementService;
class EnrollmentHandlerChromeOS;
class EnterpriseInstallAttributes;

class DeviceCloudPolicyManagerChromeOS : public CloudPolicyManager {
 public:
  typedef std::bitset<32> AllowedDeviceModes;
  typedef base::Callback<void(EnrollmentStatus)> EnrollmentCallback;

  
  
  
  DeviceCloudPolicyManagerChromeOS(
      scoped_ptr<DeviceCloudPolicyStoreChromeOS> store,
      const scoped_refptr<base::SequencedTaskRunner>& task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& background_task_runner,
      EnterpriseInstallAttributes* install_attributes);
  virtual ~DeviceCloudPolicyManagerChromeOS();

  
  void Connect(
      PrefService* local_state,
      DeviceManagementService* device_management_service,
      scoped_ptr<CloudPolicyClient::StatusProvider> device_status_provider);

  
  
  
  
  void StartEnrollment(const std::string& auth_token,
                       bool is_auto_enrollment,
                       const AllowedDeviceModes& allowed_modes,
                       const EnrollmentCallback& callback);

  
  void CancelEnrollment();

  
  std::string GetDeviceRequisition() const;
  void SetDeviceRequisition(const std::string& requisition);

  
  bool ShouldAutoStartEnrollment() const;

  
  bool CanExitEnrollment() const;

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnStoreLoaded(CloudPolicyStore* store) OVERRIDE;

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  static std::string GetMachineID();

  
  static std::string GetMachineModel();

  
  
  
  std::string GetRobotAccountId();

 private:
  
  scoped_ptr<CloudPolicyClient> CreateClient();

  
  
  void StartIfManaged();

  
  void EnrollmentCompleted(const EnrollmentCallback& callback,
                           EnrollmentStatus status);

  
  void InitalizeRequisition();

  
  
  scoped_ptr<DeviceCloudPolicyStoreChromeOS> device_store_;
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;
  EnterpriseInstallAttributes* install_attributes_;

  DeviceManagementService* device_management_service_;
  scoped_ptr<CloudPolicyClient::StatusProvider> device_status_provider_;

  
  PrefService* local_state_;

  
  scoped_ptr<EnrollmentHandlerChromeOS> enrollment_handler_;

  scoped_ptr<chromeos::attestation::AttestationPolicyObserver>
      attestation_policy_observer_;

  DISALLOW_COPY_AND_ASSIGN(DeviceCloudPolicyManagerChromeOS);
};

}  

#endif  
