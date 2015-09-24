// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_ENTERPRISE_INSTALL_ATTRIBUTES_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_ENTERPRISE_INSTALL_ATTRIBUTES_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/cryptohome_client.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"

namespace policy {

class EnterpriseInstallAttributes {
 public:
  
  enum LockResult {
    LOCK_SUCCESS,
    LOCK_NOT_READY,
    LOCK_BACKEND_ERROR,
    LOCK_WRONG_USER,
  };

  
  typedef base::Callback<void(LockResult lock_result)> LockResultCallback;

  
  static const char kConsumerDeviceMode[];
  static const char kEnterpiseDeviceMode[];
  static const char kRetailKioskDeviceMode[];
  static const char kConsumerKioskDeviceMode[];
  static const char kUnknownDeviceMode[];

  
  static const char kAttrEnterpriseDeviceId[];
  static const char kAttrEnterpriseDomain[];
  static const char kAttrEnterpriseMode[];
  static const char kAttrEnterpriseOwned[];
  static const char kAttrEnterpriseUser[];
  static const char kAttrConsumerKioskEnabled[];

  explicit EnterpriseInstallAttributes(
      chromeos::CryptohomeClient* cryptohome_client);
  ~EnterpriseInstallAttributes();

  
  
  
  void ReadCacheFile(const base::FilePath& cache_file);

  
  
  
  
  void ReadImmutableAttributes(const base::Closure& callback);

  
  
  
  
  void LockDevice(const std::string& user,
                  DeviceMode device_mode,
                  const std::string& device_id,
                  const LockResultCallback& callback);

  
  bool IsEnterpriseDevice();

  
  bool IsConsumerKioskDevice();

  
  
  std::string GetDomain();

  
  
  std::string GetRegistrationUser();

  
  
  
  std::string GetDeviceId();

  
  
  DeviceMode GetMode();

 protected:
  bool device_locked_;
  std::string registration_user_;
  std::string registration_domain_;
  std::string registration_device_id_;
  DeviceMode registration_mode_;

 private:
  
  void DecodeInstallAttributes(
      const std::map<std::string, std::string>& attr_map);

  
  void ReadAttributesIfReady(
      const base::Closure& callback,
      chromeos::DBusMethodCallStatus call_status,
      bool result);

  
  
  void LockDeviceIfAttributesIsReady(
      const std::string& user,
      DeviceMode device_mode,
      const std::string& device_id,
      const LockResultCallback& callback,
      chromeos::DBusMethodCallStatus call_status,
      bool result);

  
  void OnReadImmutableAttributes(const std::string& user,
                                 const LockResultCallback& callback);

  chromeos::CryptohomeClient* cryptohome_client_;

  base::WeakPtrFactory<EnterpriseInstallAttributes> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(EnterpriseInstallAttributes);
};

}  

#endif  
