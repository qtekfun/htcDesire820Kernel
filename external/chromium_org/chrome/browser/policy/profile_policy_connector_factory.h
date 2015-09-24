// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_PROFILE_POLICY_CONNECTOR_FACTORY_H_
#define CHROME_BROWSER_POLICY_PROFILE_POLICY_CONNECTOR_FACTORY_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_base_factory.h"

template <typename T>
struct DefaultSingletonTraits;

class Profile;

namespace base {
class SequencedTaskRunner;
}

namespace content {
class BrowserContext;
}

namespace policy {

class ProfilePolicyConnector;

class ProfilePolicyConnectorFactory : public BrowserContextKeyedBaseFactory {
 public:
  
  static ProfilePolicyConnectorFactory* GetInstance();

  
  
  static ProfilePolicyConnector* GetForProfile(Profile* profile);

  
  
  
  
  
  static scoped_ptr<ProfilePolicyConnector> CreateForProfile(
      Profile* profile,
      bool force_immediate_load);

  
  
  
  void SetServiceForTesting(Profile* profile,
                            ProfilePolicyConnector* connector);

 private:
  friend struct DefaultSingletonTraits<ProfilePolicyConnectorFactory>;

  ProfilePolicyConnectorFactory();
  virtual ~ProfilePolicyConnectorFactory();

  ProfilePolicyConnector* GetForProfileInternal(Profile* profile);

  scoped_ptr<ProfilePolicyConnector> CreateForProfileInternal(
      Profile* profile,
      bool force_immediate_load);

  
  virtual void BrowserContextShutdown(
      content::BrowserContext* context) OVERRIDE;
  virtual void BrowserContextDestroyed(
      content::BrowserContext* context) OVERRIDE;
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
  virtual void SetEmptyTestingFactory(
      content::BrowserContext* context) OVERRIDE;
  virtual void CreateServiceNow(content::BrowserContext* context) OVERRIDE;

  typedef std::map<Profile*, ProfilePolicyConnector*> ConnectorMap;
  ConnectorMap connectors_;

  DISALLOW_COPY_AND_ASSIGN(ProfilePolicyConnectorFactory);
};

}  

#endif  
