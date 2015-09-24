// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_HANDLER_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_HANDLER_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/policy/device_cloud_policy_manager_chromeos.h"
#include "chrome/browser/chromeos/policy/device_cloud_policy_validator.h"
#include "chrome/browser/chromeos/policy/enterprise_install_attributes.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "google_apis/gaia/gaia_oauth_client.h"

namespace base {
class SequencedTaskRunner;
}

namespace chromeos {
class DeviceOAuth2TokenService;
}

namespace enterprise_management {
class PolicyFetchResponse;
}

namespace policy {

class EnrollmentHandlerChromeOS : public CloudPolicyClient::Observer,
                                  public CloudPolicyStore::Observer,
                                  public gaia::GaiaOAuthClient::Delegate {
 public:
  typedef DeviceCloudPolicyManagerChromeOS::AllowedDeviceModes
      AllowedDeviceModes;
  typedef DeviceCloudPolicyManagerChromeOS::EnrollmentCallback
      EnrollmentCallback;

  
  
  
  
  
  EnrollmentHandlerChromeOS(
      DeviceCloudPolicyStoreChromeOS* store,
      EnterpriseInstallAttributes* install_attributes,
      scoped_ptr<CloudPolicyClient> client,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner,
      const std::string& auth_token,
      const std::string& client_id,
      bool is_auto_enrollment,
      const std::string& requisition,
      const AllowedDeviceModes& allowed_device_modes,
      const EnrollmentCallback& completion_callback);
  virtual ~EnrollmentHandlerChromeOS();

  
  
  void StartEnrollment();

  
  scoped_ptr<CloudPolicyClient> ReleaseClient();

  
  virtual void OnPolicyFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRegistrationStateChanged(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRobotAuthCodesFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnClientError(CloudPolicyClient* client) OVERRIDE;

  
  virtual void OnStoreLoaded(CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(CloudPolicyStore* store) OVERRIDE;

  
  virtual void OnGetTokensResponse(const std::string& refresh_token,
                                   const std::string& access_token,
                                   int expires_in_seconds) OVERRIDE;
  virtual void OnRefreshTokenResponse(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnOAuthError() OVERRIDE;
  virtual void OnNetworkError(int response_code) OVERRIDE;

 private:
  
  
  enum EnrollmentStep {
    STEP_PENDING,             
    STEP_LOADING_STORE,       
    STEP_REGISTRATION,        
    STEP_POLICY_FETCH,        
    STEP_VALIDATION,          
    STEP_ROBOT_AUTH_FETCH,    
    STEP_ROBOT_AUTH_REFRESH,  
    STEP_LOCK_DEVICE,         
    STEP_STORE_ROBOT_AUTH,    
    STEP_STORE_POLICY,        
    STEP_FINISHED,            
  };

  
  void AttemptRegistration();

  
  
  void PolicyValidated(DeviceCloudPolicyValidator* validator);

  
  
  
  void StartLockDevice(const std::string& user,
                       DeviceMode device_mode,
                       const std::string& device_id);

  
  
  void HandleLockDeviceResult(
      const std::string& user,
      DeviceMode device_mode,
      const std::string& device_id,
      EnterpriseInstallAttributes::LockResult lock_result);

  
  void Stop();

  
  void ReportResult(EnrollmentStatus status);

  
  void DidGetTokenService(chromeos::DeviceOAuth2TokenService* token_service);

  DeviceCloudPolicyStoreChromeOS* store_;
  EnterpriseInstallAttributes* install_attributes_;
  scoped_ptr<CloudPolicyClient> client_;
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;
  scoped_ptr<gaia::GaiaOAuthClient> gaia_oauth_client_;

  std::string auth_token_;
  std::string client_id_;
  bool is_auto_enrollment_;
  std::string requisition_;
  std::string refresh_token_;
  AllowedDeviceModes allowed_device_modes_;
  EnrollmentCallback completion_callback_;

  
  DeviceMode device_mode_;

  
  scoped_ptr<enterprise_management::PolicyFetchResponse> policy_;
  std::string username_;
  std::string device_id_;

  
  EnrollmentStep enrollment_step_;

  
  
  int lockbox_init_duration_;

  
  base::WeakPtrFactory<EnrollmentHandlerChromeOS> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(EnrollmentHandlerChromeOS);
};

}  

#endif  
