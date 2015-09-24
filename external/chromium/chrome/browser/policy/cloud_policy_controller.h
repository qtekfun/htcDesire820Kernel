// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_POLICY_CONTROLLER_H_
#define CHROME_BROWSER_POLICY_CLOUD_POLICY_CONTROLLER_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/task.h"
#include "base/time.h"
#include "chrome/browser/policy/cloud_policy_identity_strategy.h"
#include "chrome/browser/policy/configuration_policy_provider.h"
#include "chrome/browser/policy/device_management_backend.h"
#include "chrome/browser/policy/device_token_fetcher.h"

class Profile;
class TokenService;

namespace policy {

class CloudPolicyCacheBase;
class DeviceManagementBackend;

class CloudPolicyController
    : public DeviceManagementBackend::DevicePolicyResponseDelegate,
      public DeviceTokenFetcher::Observer,
      public CloudPolicyIdentityStrategy::Observer {
 public:
  
  CloudPolicyController(DeviceManagementService* service,
                        CloudPolicyCacheBase* cache,
                        DeviceTokenFetcher* token_fetcher,
                        CloudPolicyIdentityStrategy* identity_strategy,
                        PolicyNotifier* notifier);
  virtual ~CloudPolicyController();

  
  void SetRefreshRate(int64 refresh_rate_milliseconds);

  
  void Retry();

  
  
  void StopAutoRetry();

  
  virtual void HandlePolicyResponse(
      const em::DevicePolicyResponse& response);
  virtual void OnError(DeviceManagementBackend::ErrorCode code);

  
  virtual void OnDeviceTokenAvailable();

  
  virtual void OnDeviceTokenChanged();
  virtual void OnCredentialsChanged();

 private:
  
  enum ControllerState {
    
    STATE_TOKEN_UNAVAILABLE,
    
    STATE_TOKEN_UNMANAGED,
    
    STATE_TOKEN_ERROR,
    
    STATE_TOKEN_VALID,
    
    STATE_POLICY_VALID,
    
    STATE_POLICY_ERROR,
    
    STATE_POLICY_UNAVAILABLE
  };

  friend class CloudPolicyControllerTest;

  
  CloudPolicyController(DeviceManagementService* service,
                        CloudPolicyCacheBase* cache,
                        DeviceTokenFetcher* token_fetcher,
                        CloudPolicyIdentityStrategy* identity_strategy,
                        PolicyNotifier* notifier,
                        int64 policy_refresh_rate_ms,
                        int policy_refresh_deviation_factor_percent,
                        int64 policy_refresh_deviation_max_ms,
                        int64 policy_refresh_error_delay_ms);

  
  void Initialize(DeviceManagementService* service,
                  CloudPolicyCacheBase* cache,
                  DeviceTokenFetcher* token_fetcher,
                  CloudPolicyIdentityStrategy* identity_strategy,
                  PolicyNotifier* notifier,
                  int64 policy_refresh_rate_ms,
                  int policy_refresh_deviation_factor_percent,
                  int64 policy_refresh_deviation_max_ms,
                  int64 policy_refresh_error_delay_ms);

  
  void FetchToken();

  
  
  void SendPolicyRequest();

  
  void DoDelayedWork();

  
  
  void DoWork();

  
  void CancelDelayedWork();

  
  void SetState(ControllerState new_state);

  
  int64 GetRefreshDelay();

  DeviceManagementService* service_;
  CloudPolicyCacheBase* cache_;
  CloudPolicyIdentityStrategy* identity_strategy_;
  DeviceTokenFetcher* token_fetcher_;
  scoped_ptr<DeviceManagementBackend> backend_;
  ControllerState state_;
  PolicyNotifier* notifier_;

  int64 policy_refresh_rate_ms_;
  int policy_refresh_deviation_factor_percent_;
  int64 policy_refresh_deviation_max_ms_;
  int64 policy_refresh_error_delay_ms_;
  int64 effective_policy_refresh_error_delay_ms_;

  CancelableTask* delayed_work_task_;
  ScopedRunnableMethodFactory<CloudPolicyController> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyController);
};

}  

#endif  
