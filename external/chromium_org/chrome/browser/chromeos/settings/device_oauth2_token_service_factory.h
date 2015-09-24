// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_OAUTH2_TOKEN_SERVICE_FACTORY_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_OAUTH2_TOKEN_SERVICE_FACTORY_H_

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"

namespace chromeos {

class DeviceOAuth2TokenService;

class DeviceOAuth2TokenServiceFactory {
 public:
  
  typedef base::Callback<void(DeviceOAuth2TokenService*)> GetCallback;

  
  
  
  
  
  
  
  
  
  
  
  static void Get(const GetCallback& callback);

  
  
  
  static void Initialize();

  
  
  
  
  static void Shutdown();

 private:
  DeviceOAuth2TokenServiceFactory();
  ~DeviceOAuth2TokenServiceFactory();

  
  
  
  
  void CreateTokenService();

  
  
  void DidGetSystemSalt(const std::string& system_salt);

  
  
  
  
  void RunAsync(const GetCallback& callback);

  
  
  bool initialized_;
  DeviceOAuth2TokenService* token_service_;
  std::queue<GetCallback> pending_callbacks_;
  base::WeakPtrFactory<DeviceOAuth2TokenServiceFactory> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DeviceOAuth2TokenServiceFactory);
};

}  

#endif  
