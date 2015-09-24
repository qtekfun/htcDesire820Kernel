// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_DEVICE_POLICY_IDENTITY_STRATEGY_H_
#define CHROME_BROWSER_POLICY_DEVICE_POLICY_IDENTITY_STRATEGY_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/policy/cloud_policy_identity_strategy.h"

class TokenService;

namespace policy {

class DevicePolicyIdentityStrategy : public CloudPolicyIdentityStrategy {
 public:
  DevicePolicyIdentityStrategy();
  virtual ~DevicePolicyIdentityStrategy();

  
  
  
  void SetAuthCredentials(const std::string& username,
                          const std::string& auth_token);

  
  
  void SetDeviceManagementCredentials(const std::string& owner_email,
                                      const std::string& device_id,
                                      const std::string& device_token);

  
  
  
  void FetchPolicy();

  
  virtual std::string GetDeviceToken() OVERRIDE;
  virtual std::string GetDeviceID() OVERRIDE;
  virtual std::string GetMachineID() OVERRIDE;
  virtual std::string GetMachineModel() OVERRIDE;
  virtual em::DeviceRegisterRequest_Type GetPolicyRegisterType() OVERRIDE;
  virtual std::string GetPolicyType() OVERRIDE;
  virtual bool GetCredentials(std::string* username,
                              std::string* auth_token) OVERRIDE;
  virtual void OnDeviceTokenAvailable(const std::string& token) OVERRIDE;

 private:
  
  std::string username_;
  std::string auth_token_;

  
  std::string machine_id_;
  std::string machine_model_;

  
  
  
  std::string device_id_;

  
  std::string device_token_;

  DISALLOW_COPY_AND_ASSIGN(DevicePolicyIdentityStrategy);
};

}  

#endif  
