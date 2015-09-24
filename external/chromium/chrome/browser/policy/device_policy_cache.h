// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_DEVICE_POLICY_CACHE_H_
#define CHROME_BROWSER_POLICY_DEVICE_POLICY_CACHE_H_
#pragma once

#include <string>

#include "base/memory/scoped_callback_factory.h"
#include "chrome/browser/chromeos/login/signed_settings.h"
#include "chrome/browser/chromeos/login/signed_settings_helper.h"
#include "chrome/browser/policy/cloud_policy_cache_base.h"
#include "chrome/browser/policy/proto/chrome_device_policy.pb.h"

namespace policy {

class DevicePolicyIdentityStrategy;
class EnterpriseInstallAttributes;
class PolicyMap;

namespace em = enterprise_management;

class DevicePolicyCache : public CloudPolicyCacheBase,
                          public chromeos::SignedSettingsHelper::Callback {
 public:
  explicit DevicePolicyCache(DevicePolicyIdentityStrategy* identity_strategy,
                             EnterpriseInstallAttributes* install_attributes);
  virtual ~DevicePolicyCache();

  
  virtual void Load() OVERRIDE;
  virtual void SetPolicy(const em::PolicyFetchResponse& policy) OVERRIDE;
  virtual void SetUnmanaged() OVERRIDE;

  
  virtual void OnRetrievePolicyCompleted(
      chromeos::SignedSettings::ReturnCode code,
      const em::PolicyFetchResponse& policy) OVERRIDE;

 private:
  friend class DevicePolicyCacheTest;

  
  
  DevicePolicyCache(
      DevicePolicyIdentityStrategy* identity_strategy,
      EnterpriseInstallAttributes* install_attributes,
      chromeos::SignedSettingsHelper* signed_settings_helper);

  
  virtual bool DecodePolicyData(const em::PolicyData& policy_data,
                                PolicyMap* mandatory,
                                PolicyMap* recommended) OVERRIDE;

  void PolicyStoreOpCompleted(chromeos::SignedSettings::ReturnCode code);

  
  
  void CheckImmutableAttributes();

  static void DecodeDevicePolicy(const em::ChromeDeviceSettingsProto& policy,
                                 PolicyMap* mandatory,
                                 PolicyMap* recommended);

  DevicePolicyIdentityStrategy* identity_strategy_;
  EnterpriseInstallAttributes* install_attributes_;

  chromeos::SignedSettingsHelper* signed_settings_helper_;

  bool starting_up_;

  base::ScopedCallbackFactory<DevicePolicyCache> callback_factory_;

  DISALLOW_COPY_AND_ASSIGN(DevicePolicyCache);
};

}  

#endif  
