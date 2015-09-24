// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_LOCAL_ACCOUNT_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_LOCAL_ACCOUNT_H_

#include <string>
#include <vector>

namespace chromeos {
class CrosSettings;
}

namespace policy {

struct DeviceLocalAccount {
  enum Type {
    
    TYPE_PUBLIC_SESSION,
    
    TYPE_KIOSK_APP,
    
    TYPE_COUNT
  };

  DeviceLocalAccount(Type type,
                     const std::string& account_id,
                     const std::string& kiosk_app_id);
  ~DeviceLocalAccount();

  Type type;
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::string account_id;
  std::string user_id;
  std::string kiosk_app_id;
};

std::string GenerateDeviceLocalAccountUserId(const std::string& account_id,
                                             DeviceLocalAccount::Type type);

bool IsDeviceLocalAccountUser(const std::string& user_id,
                              DeviceLocalAccount::Type* type);

void SetDeviceLocalAccounts(
    chromeos::CrosSettings* cros_settings,
    const std::vector<DeviceLocalAccount>& accounts);

std::vector<DeviceLocalAccount> GetDeviceLocalAccounts(
    chromeos::CrosSettings* cros_settings);

}  

#endif  
