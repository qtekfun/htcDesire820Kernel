// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_USER_POLICY_SIGNIN_SERVICE_FACTORY_H_
#define CHROME_BROWSER_POLICY_CLOUD_USER_POLICY_SIGNIN_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace policy {

class DeviceManagementService;
class UserPolicySigninService;

class UserPolicySigninServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  static UserPolicySigninServiceFactory* GetInstance();

  
  
  static UserPolicySigninService* GetForProfile(Profile* profile);

  
  
  
  static void SetDeviceManagementServiceForTesting(
      DeviceManagementService* device_management_service);

 protected:
  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;

  
  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;

  
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<UserPolicySigninServiceFactory>;

  UserPolicySigninServiceFactory();
  virtual ~UserPolicySigninServiceFactory();

  DISALLOW_COPY_AND_ASSIGN(UserPolicySigninServiceFactory);
};

}  

#endif  
