// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_REGISTRATION_UTILITY_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_REGISTRATION_UTILITY_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/strings/string16.h"
#include "chrome/browser/managed_mode/managed_user_sync_service.h"
#include "chrome/browser/managed_mode/managed_user_sync_service_observer.h"
#include "chrome/browser/managed_mode/managed_users.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class GoogleServiceAuthError;
class ManagedUserRefreshTokenFetcher;
class ManagedUserRegistrationUtilityTest;
class PrefService;
class Profile;

namespace browser_sync {
class DeviceInfo;
}

struct ManagedUserRegistrationInfo {
  ManagedUserRegistrationInfo(const base::string16& name, int avatar_index);
  int avatar_index;
  base::string16 name;
  std::string master_key;
};

class ManagedUserRegistrationUtility {
 public:
  
  
  
  
  typedef base::Callback<void(const GoogleServiceAuthError& ,
                              const std::string& )>
      RegistrationCallback;

  virtual ~ManagedUserRegistrationUtility() {}

  
  static scoped_ptr<ManagedUserRegistrationUtility> Create(Profile* profile);

  static std::string GenerateNewManagedUserId();

  
  
  
  
  
  
  
  
  
  virtual void Register(const std::string& managed_user_id,
                        const ManagedUserRegistrationInfo& info,
                        const RegistrationCallback& callback) = 0;

 protected:
  ManagedUserRegistrationUtility() {}

 private:
  friend class ScopedTestingManagedUserRegistrationUtility;
  friend class ManagedUserRegistrationUtilityTest;

  
  static ManagedUserRegistrationUtility* CreateImpl(
      PrefService* prefs,
      scoped_ptr<ManagedUserRefreshTokenFetcher> token_fetcher,
      ManagedUserSyncService* service);

  
  
  static void SetUtilityForTests(ManagedUserRegistrationUtility* utility);
};

class ScopedTestingManagedUserRegistrationUtility {
 public:
  
  ScopedTestingManagedUserRegistrationUtility(
      ManagedUserRegistrationUtility* instance);

  ~ScopedTestingManagedUserRegistrationUtility();
};

#endif  
