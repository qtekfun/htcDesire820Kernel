// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_SESSION_MANAGER_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_SESSION_MANAGER_OPERATION_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/policy/device_cloud_policy_validator.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"

namespace enterprise_management {
class ChromeDeviceSettingsProto;
class PolicyData;
class PolicyFetchResponse;
}

namespace chromeos {

class OwnerKeyUtil;
class SessionManagerClient;

class SessionManagerOperation {
 public:
  typedef base::Callback<void(SessionManagerOperation*,
                              DeviceSettingsService::Status)> Callback;

  
  explicit SessionManagerOperation(const Callback& callback);
  virtual ~SessionManagerOperation();

  
  void Start(SessionManagerClient* session_manager_client,
             scoped_refptr<OwnerKeyUtil> owner_key_util,
             scoped_refptr<OwnerKey> owner_key);

  
  void RestartLoad(bool key_changed);

  
  scoped_ptr<enterprise_management::PolicyData>& policy_data() {
    return policy_data_;
  }
  scoped_ptr<enterprise_management::ChromeDeviceSettingsProto>&
      device_settings() {
    return device_settings_;
  }

  
  scoped_refptr<OwnerKey> owner_key() {
    return owner_key_;
  }

  
  bool is_loading() const {
    return is_loading_;
  }

  void set_force_key_load(bool force_key_load) {
    force_key_load_ = force_key_load;
  }

 protected:
  
  virtual void Run() = 0;

  
  void EnsureOwnerKey(const base::Closure& callback);

  
  void StartLoading();

  
  
  void ReportResult(DeviceSettingsService::Status status);

  SessionManagerClient* session_manager_client() {
    return session_manager_client_;
  }

 private:
  
  static scoped_refptr<OwnerKey> LoadOwnerKey(
      scoped_refptr<OwnerKeyUtil> util,
      scoped_refptr<OwnerKey> current_key);

  
  void StoreOwnerKey(const base::Closure& callback,
                     scoped_refptr<OwnerKey> new_key);

  
  void RetrieveDeviceSettings();

  
  void ValidateDeviceSettings(const std::string& policy_blob);

  
  void ReportValidatorStatus(policy::DeviceCloudPolicyValidator* validator);

  SessionManagerClient* session_manager_client_;
  scoped_refptr<OwnerKeyUtil> owner_key_util_;

  base::WeakPtrFactory<SessionManagerOperation> weak_factory_;

  Callback callback_;

  scoped_refptr<OwnerKey> owner_key_;
  bool force_key_load_;

  bool is_loading_;
  scoped_ptr<enterprise_management::PolicyData> policy_data_;
  scoped_ptr<enterprise_management::ChromeDeviceSettingsProto> device_settings_;

  DISALLOW_COPY_AND_ASSIGN(SessionManagerOperation);
};

class LoadSettingsOperation : public SessionManagerOperation {
 public:
  
  explicit LoadSettingsOperation(const Callback& callback);
  virtual ~LoadSettingsOperation();

 protected:
  
  virtual void Run() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(LoadSettingsOperation);
};

class StoreSettingsOperation : public SessionManagerOperation {
 public:
  
  StoreSettingsOperation(
      const Callback& callback,
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy);
  virtual ~StoreSettingsOperation();

 protected:
  
  virtual void Run() OVERRIDE;

 private:
  
  void HandleStoreResult(bool success);

  scoped_ptr<enterprise_management::PolicyFetchResponse> policy_;

  base::WeakPtrFactory<StoreSettingsOperation> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(StoreSettingsOperation);
};

class SignAndStoreSettingsOperation : public SessionManagerOperation {
 public:
  
  SignAndStoreSettingsOperation(
      const Callback& callback,
      scoped_ptr<enterprise_management::ChromeDeviceSettingsProto> new_settings,
      const std::string& username);
  virtual ~SignAndStoreSettingsOperation();

  
  virtual void Run() OVERRIDE;

 private:
  
  void StartSigning();

  
  static std::string AssembleAndSignPolicy(
      scoped_ptr<enterprise_management::ChromeDeviceSettingsProto> settings,
      const std::string& username,
      scoped_refptr<OwnerKey> owner_key);

  
  void StoreDeviceSettingsBlob(std::string device_settings_blob);

  
  void HandleStoreResult(bool success);

  scoped_ptr<enterprise_management::ChromeDeviceSettingsProto> new_settings_;
  std::string username_;

  base::WeakPtrFactory<SignAndStoreSettingsOperation> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(SignAndStoreSettingsOperation);
};

}  

#endif  
