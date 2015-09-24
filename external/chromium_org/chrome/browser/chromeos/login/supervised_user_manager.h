// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SUPERVISED_USER_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SUPERVISED_USER_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/strings/string16.h"
#include "chrome/browser/profiles/profile.h"

class PrefRegistrySimple;

namespace chromeos {

class User;

class SupervisedUserManager {
 public:
  typedef base::Callback<void(const std::string& )>
      LoadTokenCallback;

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  SupervisedUserManager() {}
  virtual ~SupervisedUserManager() {}

  
  
  
  
  
  
  virtual const User* CreateUserRecord(
      const std::string& manager_id,
      const std::string& local_user_id,
      const std::string& sync_user_id,
      const base::string16& display_name) = 0;

  
  virtual std::string GenerateUserId() = 0;

  
  
  virtual const User* FindByDisplayName(
      const base::string16& display_name) const = 0;

  
  
  virtual const User* FindBySyncId(const std::string& sync_id) const = 0;

  
  
  virtual std::string GetUserSyncId(const std::string& user_id) const = 0;

  
  
  
  virtual base::string16 GetManagerDisplayName(
      const std::string& user_id) const = 0;

  
  
  
  virtual std::string GetManagerUserId(const std::string& user_id) const = 0;

  
  
  
  virtual std::string GetManagerDisplayEmail(const std::string& user_id)
      const = 0;

  
  virtual void StartCreationTransaction(const base::string16& display_name) = 0;

  
  virtual void SetCreationTransactionUserId(const std::string& user_id) = 0;

  
  virtual void CommitCreationTransaction() = 0;

  
  virtual void LoadSupervisedUserToken(Profile* profile,
                                       const LoadTokenCallback& callback) = 0;

  
  virtual void ConfigureSyncWithToken(Profile* profile,
                                      const std::string& token) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SupervisedUserManager);
};

}  

#endif  
