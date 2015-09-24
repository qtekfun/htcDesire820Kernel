// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_POLICY_IDENTITY_STRATEGY_H_
#define CHROME_BROWSER_POLICY_CLOUD_POLICY_IDENTITY_STRATEGY_H_
#pragma once

#include <string>

#include "base/observer_list.h"
#include "chrome/browser/policy/proto/device_management_backend.pb.h"

namespace policy {

namespace em = enterprise_management;

class CloudPolicyIdentityStrategy {
 public:
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void OnDeviceTokenChanged() = 0;

    
    
    virtual void OnCredentialsChanged() = 0;
  };

  CloudPolicyIdentityStrategy();
  virtual ~CloudPolicyIdentityStrategy();

  void AddObserver(Observer* obs);
  void RemoveObserver(Observer* obs);

  
  
  virtual std::string GetDeviceToken() = 0;

  
  
  
  virtual std::string GetDeviceID() = 0;

  
  virtual std::string GetMachineID() = 0;

  
  virtual std::string GetMachineModel() = 0;

  
  
  virtual em::DeviceRegisterRequest_Type GetPolicyRegisterType() = 0;

  
  
  virtual std::string GetPolicyType() = 0;

  
  
  
  virtual bool GetCredentials(std::string* username,
                              std::string* auth_token) = 0;

  
  
  
  
  virtual void OnDeviceTokenAvailable(const std::string& token) = 0;

 protected:
  
  void NotifyDeviceTokenChanged();

  
  void NotifyAuthChanged();

 private:
  ObserverList<Observer, true> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyIdentityStrategy);
};

}  

#endif  
