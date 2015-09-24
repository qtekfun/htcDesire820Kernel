// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MULTI_PROFILE_USER_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MULTI_PROFILE_USER_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"

class PrefChangeRegistrar;
class PrefRegistrySimple;
class PrefService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chromeos {

class MultiProfileUserControllerDelegate;
class UserManager;

class MultiProfileUserController {
 public:
  MultiProfileUserController(MultiProfileUserControllerDelegate* delegate,
                             PrefService* local_state);
  ~MultiProfileUserController();

  static void RegisterPrefs(PrefRegistrySimple* registry);
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  bool IsUserAllowedInSession(const std::string& user_email) const;

  
  void StartObserving(Profile* user_profile);

  
  void RemoveCachedValues(const std::string& user_email);

  
  static const char kBehaviorUnrestricted[];
  static const char kBehaviorPrimaryOnly[];
  static const char kBehaviorNotAllowed[];

 private:
  friend class MultiProfileUserControllerTest;

  
  std::string GetCachedValue(const std::string& user_email) const;
  void SetCachedValue(const std::string& user_email,
                      const std::string& behavior);

  
  void CheckSessionUsers();

  
  void OnUserPrefChanged(Profile* profile);

  MultiProfileUserControllerDelegate* delegate_;  
  PrefService* local_state_;  
  ScopedVector<PrefChangeRegistrar> pref_watchers_;

  DISALLOW_COPY_AND_ASSIGN(MultiProfileUserController);
};

}  

#endif  
