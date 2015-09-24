// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_MANAGER_FACTORY_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_MANAGER_FACTORY_CHROMEOS_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_base_factory.h"

class Profile;

namespace base {
class SequencedTaskRunner;
}

namespace content {
class BrowserContext;
}

namespace policy {

class UserCloudPolicyManagerChromeOS;

class UserCloudPolicyManagerFactoryChromeOS
    : public BrowserContextKeyedBaseFactory {
 public:
  
  static UserCloudPolicyManagerFactoryChromeOS* GetInstance();

  
  
  static UserCloudPolicyManagerChromeOS* GetForProfile(Profile* profile);

  
  
  
  
  
  
  static scoped_ptr<UserCloudPolicyManagerChromeOS> CreateForProfile(
      Profile* profile,
      bool force_immediate_load,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

 private:
  friend struct DefaultSingletonTraits<UserCloudPolicyManagerFactoryChromeOS>;

  UserCloudPolicyManagerFactoryChromeOS();
  virtual ~UserCloudPolicyManagerFactoryChromeOS();

  
  UserCloudPolicyManagerChromeOS* GetManagerForProfile(Profile* profile);
  scoped_ptr<UserCloudPolicyManagerChromeOS> CreateManagerForProfile(
      Profile* profile,
      bool force_immediate_load,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

  
  virtual void BrowserContextShutdown(
      content::BrowserContext* context) OVERRIDE;
  virtual void BrowserContextDestroyed(
      content::BrowserContext* context) OVERRIDE;
  virtual void SetEmptyTestingFactory(
      content::BrowserContext* context) OVERRIDE;
  virtual void CreateServiceNow(content::BrowserContext* context) OVERRIDE;

  typedef std::map<Profile*, UserCloudPolicyManagerChromeOS*> ManagerMap;
  ManagerMap managers_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudPolicyManagerFactoryChromeOS);
};

}  

#endif  
