// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_LOCAL_ACCOUNT_POLICY_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_LOCAL_ACCOUNT_POLICY_PROVIDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/policy/device_local_account_external_data_manager.h"
#include "chrome/browser/chromeos/policy/device_local_account_policy_service.h"
#include "components/policy/core/common/configuration_policy_provider.h"

namespace policy {

class DeviceLocalAccountPolicyProvider
    : public ConfigurationPolicyProvider,
      public DeviceLocalAccountPolicyService::Observer {
 public:
  DeviceLocalAccountPolicyProvider(const std::string& user_id,
                                   DeviceLocalAccountPolicyService* service);
  virtual ~DeviceLocalAccountPolicyProvider();

  
  virtual bool IsInitializationComplete(PolicyDomain domain) const OVERRIDE;
  virtual void RefreshPolicies() OVERRIDE;

  
  virtual void OnPolicyUpdated(const std::string& user_id) OVERRIDE;
  virtual void OnDeviceLocalAccountsChanged() OVERRIDE;

 private:
  
  DeviceLocalAccountPolicyBroker* GetBroker();

  
  
  void ReportPolicyRefresh(bool success);

  
  
  void UpdateFromBroker();

  const std::string user_id_;
  scoped_refptr<DeviceLocalAccountExternalDataManager> external_data_manager_;

  DeviceLocalAccountPolicyService* service_;

  bool store_initialized_;
  bool waiting_for_policy_refresh_;

  base::WeakPtrFactory<DeviceLocalAccountPolicyProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DeviceLocalAccountPolicyProvider);
};

}  

#endif  
