// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_DEVICE_MANAGEMENT_BACKEND_IMPL_H_
#define CHROME_BROWSER_POLICY_DEVICE_MANAGEMENT_BACKEND_IMPL_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/policy/device_management_backend.h"

namespace policy {

class DeviceManagementJobBase;
class DeviceManagementService;

class DeviceManagementBackendImpl : public DeviceManagementBackend {
 public:
  explicit DeviceManagementBackendImpl(DeviceManagementService* service);
  virtual ~DeviceManagementBackendImpl();

  static std::string GetAgentString();
  static std::string GetPlatformString();

  
  static const char kParamRequest[];
  static const char kParamDeviceType[];
  static const char kParamAppType[];
  static const char kParamDeviceID[];
  static const char kParamAgent[];
  static const char kParamPlatform[];

  
  static const char kValueRequestRegister[];
  static const char kValueRequestUnregister[];
  static const char kValueRequestPolicy[];
  static const char kValueDeviceType[];
  static const char kValueAppType[];

 private:
  friend class DeviceManagementJobBase;

  typedef std::set<DeviceManagementJobBase*> JobSet;

  
  void JobDone(DeviceManagementJobBase* job);

  
  
  void AddJob(DeviceManagementJobBase* job);

  
  virtual void ProcessRegisterRequest(
      const std::string& auth_token,
      const std::string& device_id,
      const em::DeviceRegisterRequest& request,
      DeviceRegisterResponseDelegate* response_delegate);
  virtual void ProcessUnregisterRequest(
      const std::string& device_management_token,
      const std::string& device_id,
      const em::DeviceUnregisterRequest& request,
      DeviceUnregisterResponseDelegate* response_delegate);
  virtual void ProcessPolicyRequest(
      const std::string& device_management_token,
      const std::string& device_id,
      const em::DevicePolicyRequest& request,
      DevicePolicyResponseDelegate* response_delegate);

  
  JobSet pending_jobs_;

  DeviceManagementService* service_;

  DISALLOW_COPY_AND_ASSIGN(DeviceManagementBackendImpl);
};

}  

#endif  
