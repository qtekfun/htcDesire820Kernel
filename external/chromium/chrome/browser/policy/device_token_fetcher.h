// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_DEVICE_TOKEN_FETCHER_H_
#define CHROME_BROWSER_POLICY_DEVICE_TOKEN_FETCHER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/task.h"
#include "chrome/browser/policy/device_management_backend.h"
#include "chrome/browser/policy/policy_notifier.h"
#include "chrome/browser/policy/proto/device_management_backend.pb.h"

namespace policy {

class CloudPolicyCacheBase;
class DeviceManagementService;

namespace em = enterprise_management;

class DeviceTokenFetcher
    : public DeviceManagementBackend::DeviceRegisterResponseDelegate {
 public:
  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnDeviceTokenAvailable() = 0;
  };

  
  
  DeviceTokenFetcher(DeviceManagementService* service,
                     CloudPolicyCacheBase* cache,
                     PolicyNotifier* notifier);
  
  DeviceTokenFetcher(DeviceManagementService* service,
                     CloudPolicyCacheBase* cache,
                     PolicyNotifier* notifier,
                     int64 token_fetch_error_delay_ms,
                     int64 token_fetch_error_max_delay_ms,
                     int64 unmanaged_device_refresh_rate_ms);
  virtual ~DeviceTokenFetcher();

  
  
  virtual void FetchToken(const std::string& auth_token,
                          const std::string& device_id,
                          em::DeviceRegisterRequest_Type policy_type,
                          const std::string& machine_id,
                          const std::string& machine_model);

  virtual void SetUnmanagedState();

  
  
  virtual const std::string& GetDeviceToken();

  
  void StopAutoRetry();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual void HandleRegisterResponse(
      const em::DeviceRegisterResponse& response);
  virtual void OnError(DeviceManagementBackend::ErrorCode code);

 private:
  friend class DeviceTokenFetcherTest;

  
  
  
  
  
  
  
  enum FetcherState {
    
    STATE_INACTIVE,
    
    STATE_TOKEN_AVAILABLE,
    
    STATE_UNMANAGED,
    
    STATE_ERROR,
    
    STATE_TEMPORARY_ERROR,
    
    STATE_BAD_AUTH
  };

  
  void Initialize(DeviceManagementService* service,
                  CloudPolicyCacheBase* cache,
                  PolicyNotifier* notifier,
                  int64 token_fetch_error_delay_ms,
                  int64 token_fetch_error_max_delay_ms,
                  int64 unmanaged_device_refresh_rate_ms);

  
  void SetState(FetcherState state);

  
  
  void FetchTokenInternal();

  
  void ExecuteRetryTask();

  
  void CancelRetryTask();

  
  
  DeviceManagementService* service_;  
  scoped_ptr<DeviceManagementBackend> backend_;

  
  CloudPolicyCacheBase* cache_;

  PolicyNotifier* notifier_;

  
  int64 token_fetch_error_delay_ms_;
  int64 token_fetch_error_max_delay_ms_;
  int64 effective_token_fetch_error_delay_ms_;
  int64 unmanaged_device_refresh_rate_ms_;

  
  FetcherState state_;

  
  std::string device_token_;

  
  std::string auth_token_;
  
  std::string device_id_;
  
  em::DeviceRegisterRequest_Type policy_type_;
  
  std::string machine_id_;
  
  std::string machine_model_;

  
  CancelableTask* retry_task_;

  ScopedRunnableMethodFactory<DeviceTokenFetcher> method_factory_;

  ObserverList<Observer, true> observer_list_;
};

}  

#endif  
