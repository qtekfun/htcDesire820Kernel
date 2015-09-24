// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_PROVIDER_H_

#include <deque>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_value_map.h"
#include "chrome/browser/chromeos/policy/proto/chrome_device_policy.pb.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "chromeos/settings/cros_settings_provider.h"

namespace base {
class Value;
}

namespace enterprise_management {
class ChromeDeviceSettingsProto;
}  

namespace chromeos {

class DeviceSettingsProvider : public CrosSettingsProvider,
                               public DeviceSettingsService::Observer,
                               public NetworkStateHandlerObserver {
 public:
  DeviceSettingsProvider(const NotifyObserversCallback& notify_cb,
                         DeviceSettingsService* device_settings_service);
  virtual ~DeviceSettingsProvider();

  
  static bool IsDeviceSetting(const std::string& name);

  
  virtual const base::Value* Get(const std::string& path) const OVERRIDE;
  virtual TrustedStatus PrepareTrustedValues(
      const base::Closure& callback) OVERRIDE;
  virtual bool HandlesSetting(const std::string& path) const OVERRIDE;

  
  virtual void DeviceListChanged() OVERRIDE;

 private:
  
  virtual void DoSet(const std::string& path,
                     const base::Value& value) OVERRIDE;

  
  virtual void OwnershipStatusChanged() OVERRIDE;
  virtual void DeviceSettingsUpdated() OVERRIDE;

  
  
  
  void RetrieveCachedData();

  
  
  
  void SetInPolicy();

  
  void DecodeLoginPolicies(
      const enterprise_management::ChromeDeviceSettingsProto& policy,
      PrefValueMap* new_values_cache) const;
  void DecodeKioskPolicies(
      const enterprise_management::ChromeDeviceSettingsProto& policy,
      PrefValueMap* new_values_cache) const;
  void DecodeNetworkPolicies(
      const enterprise_management::ChromeDeviceSettingsProto& policy,
      PrefValueMap* new_values_cache) const;
  void DecodeAutoUpdatePolicies(
      const enterprise_management::ChromeDeviceSettingsProto& policy,
      PrefValueMap* new_values_cache) const;
  void DecodeReportingPolicies(
      const enterprise_management::ChromeDeviceSettingsProto& policy,
      PrefValueMap* new_values_cache) const;
  void DecodeGenericPolicies(
      const enterprise_management::ChromeDeviceSettingsProto& policy,
      PrefValueMap* new_values_cache) const;

  
  void UpdateValuesCache(
      const enterprise_management::PolicyData& policy_data,
      const enterprise_management::ChromeDeviceSettingsProto& settings,
      TrustedStatus trusted_status);

  
  void ApplyMetricsSetting(bool use_file, bool new_value);

  
  void ApplyRoamingSetting(bool new_value);
  void ApplyRoamingSettingFromProto(
      const enterprise_management::ChromeDeviceSettingsProto& settings);

  
  
  void ApplySideEffects(
      const enterprise_management::ChromeDeviceSettingsProto& settings);

  
  
  
  
  bool MitigateMissingPolicy();

  
  
  TrustedStatus RequestTrustedEntity();

  
  
  void UpdateAndProceedStoring();

  
  
  
  bool UpdateFromService();

  
  
  void StoreDeviceSettings();

  
  
  
  void AttemptMigration();

  
  std::vector<base::Closure> callbacks_;

  DeviceSettingsService* device_settings_service_;
  mutable PrefValueMap migration_values_;

  TrustedStatus trusted_status_;
  DeviceSettingsService::OwnershipStatus ownership_status_;

  
  
  
  
  // been written to session_manager.
  enterprise_management::ChromeDeviceSettingsProto device_settings_;

  
  
  
  PrefValueMap values_cache_;

  
  typedef std::pair<std::string, base::Value*> PendingQueueElement;
  std::deque<PendingQueueElement> pending_changes_;

  
  base::WeakPtrFactory<DeviceSettingsProvider> store_callback_factory_;

  friend class DeviceSettingsProviderTest;
  FRIEND_TEST_ALL_PREFIXES(DeviceSettingsProviderTest,
                           InitializationTestUnowned);
  FRIEND_TEST_ALL_PREFIXES(DeviceSettingsProviderTest,
                           PolicyFailedPermanentlyNotification);
  FRIEND_TEST_ALL_PREFIXES(DeviceSettingsProviderTest, PolicyLoadNotification);
  DISALLOW_COPY_AND_ASSIGN(DeviceSettingsProvider);
};

}  

#endif  
