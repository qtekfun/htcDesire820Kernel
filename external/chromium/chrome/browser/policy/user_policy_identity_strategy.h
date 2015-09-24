// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_USER_POLICY_IDENTITY_STRATEGY_H_
#define CHROME_BROWSER_POLICY_USER_POLICY_IDENTITY_STRATEGY_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/policy/cloud_policy_identity_strategy.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;

namespace policy {

class DeviceManagementBackend;

class UserPolicyIdentityStrategy : public CloudPolicyIdentityStrategy,
                                   public NotificationObserver {
 public:
  UserPolicyIdentityStrategy(Profile* profile,
                             const FilePath& token_cache_file);
  virtual ~UserPolicyIdentityStrategy();

  
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
  class TokenCache;

  
  
  void CheckAndTriggerFetch();

  
  std::string GetCurrentUser();

  
  void OnCacheLoaded(const std::string& token, const std::string& device_id);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  Profile* profile_;

  
  scoped_refptr<TokenCache> cache_;

  
  std::string device_id_;

  
  std::string device_token_;

  
  NotificationRegistrar registrar_;

  
  base::WeakPtrFactory<UserPolicyIdentityStrategy> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(UserPolicyIdentityStrategy);
};

}  

#endif  
