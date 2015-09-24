// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_DEPENDENCY_MANAGER_H_
#define CHROME_BROWSER_PROFILES_PROFILE_DEPENDENCY_MANAGER_H_

#include <map>
#include <vector>

#include "base/memory/singleton.h"

class Profile;
class ProfileKeyedServiceFactory;

class ProfileDependencyManager {
 public:
  
  
  void AddComponent(ProfileKeyedServiceFactory* component);
  void RemoveComponent(ProfileKeyedServiceFactory* component);

  
  void AddEdge(ProfileKeyedServiceFactory* depended,
               ProfileKeyedServiceFactory* dependee);

  
  
  
  
  
  
  
  
  
  
  void DestroyProfileServices(Profile* profile);

  static ProfileDependencyManager* GetInstance();

 private:
  friend class ProfileDependencyManagerUnittests;
  friend struct DefaultSingletonTraits<ProfileDependencyManager>;

  typedef std::multimap<ProfileKeyedServiceFactory*,
                        ProfileKeyedServiceFactory*> EdgeMap;

  ProfileDependencyManager();
  virtual ~ProfileDependencyManager();

  
  
  void BuildDestructionOrder();

  std::vector<ProfileKeyedServiceFactory*> all_components_;

  EdgeMap edges_;

  std::vector<ProfileKeyedServiceFactory*> destruction_order_;
};

#endif  
