// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_KEYED_SERVICE_FACTORY_H_
#define CHROME_BROWSER_PROFILES_PROFILE_KEYED_SERVICE_FACTORY_H_

#include <map>

class Profile;
class ProfileDependencyManager;
class ProfileKeyedService;

class ProfileKeyedServiceFactory {
 public:
  typedef ProfileKeyedService* (*FactoryFunction)(Profile* profile);

#if defined(UNIT_TEST)
  
  
  
  
  void ForceAssociationBetween(Profile* profile, ProfileKeyedService* service) {
    Associate(profile, service);
  }

  
  
  
  
  void set_test_factory(FactoryFunction factory) { factory_ = factory; }
#endif

 protected:
  
  
  
  
  
  
  
  
  explicit ProfileKeyedServiceFactory(ProfileDependencyManager* manager);
  virtual ~ProfileKeyedServiceFactory();

  
  
  
  ProfileKeyedService* GetServiceForProfile(Profile* profile);

  
  
  void DependsOn(ProfileKeyedServiceFactory* rhs);

  
  void Associate(Profile* profile, ProfileKeyedService* service);

  
  
  virtual ProfileKeyedService* BuildServiceInstanceFor(
      Profile* profile) const = 0;

  
  
  
  
  virtual bool ServiceRedirectedInIncognito();
  virtual bool ServiceHasOwnInstanceInIncognito();

  
  
  
  
  
  
  
  
  
  
  
  virtual void ProfileShutdown(Profile* profile);
  virtual void ProfileDestroyed(Profile* profile);

 private:
  friend class ProfileDependencyManager;
  friend class ProfileDependencyManagerUnittests;

  
  std::map<Profile*, ProfileKeyedService*> mapping_;

  
  
  
  ProfileDependencyManager* dependency_manager_;

  
  FactoryFunction factory_;
};

#endif  
