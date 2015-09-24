// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_DEVICE_LOCAL_ACCOUNT_MANAGEMENT_POLICY_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_DEVICE_LOCAL_ACCOUNT_MANAGEMENT_POLICY_PROVIDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/policy/device_local_account.h"
#include "extensions/browser/management_policy.h"

namespace chromeos {

class DeviceLocalAccountManagementPolicyProvider
    : public extensions::ManagementPolicy::Provider {
 public:
  explicit DeviceLocalAccountManagementPolicyProvider(
      policy::DeviceLocalAccount::Type account_type);
  virtual ~DeviceLocalAccountManagementPolicyProvider();

  
  virtual std::string GetDebugPolicyProviderName() const OVERRIDE;
  virtual bool UserMayLoad(const extensions::Extension* extension,
                           base::string16* error) const OVERRIDE;

 private:
  const policy::DeviceLocalAccount::Type account_type_;

  DISALLOW_COPY_AND_ASSIGN(DeviceLocalAccountManagementPolicyProvider);
};

}  

#endif  
