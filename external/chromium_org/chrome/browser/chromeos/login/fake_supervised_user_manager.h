// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_FAKE_SUPERVISED_USER_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_FAKE_SUPERVISED_USER_MANAGER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/login/supervised_user_manager.h"

namespace chromeos {

class FakeSupervisedUserManager : public SupervisedUserManager {
 public:
  FakeSupervisedUserManager();
  virtual ~FakeSupervisedUserManager();

  virtual const User* CreateUserRecord(
      const std::string& manager_id,
      const std::string& local_user_id,
      const std::string& sync_user_id,
      const base::string16& display_name) OVERRIDE;
  virtual std::string GenerateUserId() OVERRIDE;
  virtual const User* FindByDisplayName(const base::string16& display_name) const
      OVERRIDE;
  virtual const User* FindBySyncId(const std::string& sync_id) const OVERRIDE;
  virtual std::string GetUserSyncId(const std::string& user_id) const OVERRIDE;
  virtual base::string16 GetManagerDisplayName(const std::string& user_id) const
      OVERRIDE;
  virtual std::string GetManagerUserId(const std::string& user_id) const
      OVERRIDE;
  virtual std::string GetManagerDisplayEmail(const std::string& user_id) const
      OVERRIDE;
  virtual void StartCreationTransaction(const base::string16& display_name)
      OVERRIDE {}
  virtual void SetCreationTransactionUserId(const std::string& user_id)
      OVERRIDE {}
  virtual void CommitCreationTransaction() OVERRIDE {}
  virtual void LoadSupervisedUserToken(
      Profile * profile,
      const LoadTokenCallback& callback) OVERRIDE;
  virtual void ConfigureSyncWithToken(
      Profile* profile,
      const std::string& token) OVERRIDE {}

 private:
  DISALLOW_COPY_AND_ASSIGN(FakeSupervisedUserManager);
};

}  

#endif  
