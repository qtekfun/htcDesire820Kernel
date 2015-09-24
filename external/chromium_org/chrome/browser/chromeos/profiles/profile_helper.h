// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PROFILES_PROFILE_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_PROFILES_PROFILE_HELPER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "chrome/browser/browsing_data/browsing_data_remover.h"
#include "chrome/browser/chromeos/login/oauth2_login_manager.h"
#include "chrome/browser/chromeos/login/user_manager.h"

class Profile;

namespace chromeos {

class ProfileHelper : public BrowsingDataRemover::Observer,
                      public OAuth2LoginManager::Observer,
                      public UserManager::UserSessionStateObserver {
 public:
  ProfileHelper();
  virtual ~ProfileHelper();

  
  static Profile* GetProfileByUserIdHash(const std::string& user_id_hash);

  
  static base::FilePath GetProfileDirByLegacyLoginProfileSwitch();

  
  static base::FilePath GetProfilePathByUserIdHash(
      const std::string& user_id_hash);

  
  static Profile* GetSigninProfile();

  
  
  static std::string GetUserIdHashFromProfile(Profile* profile);

  
  static base::FilePath GetUserProfileDir(const std::string& user_id_hash);

  
  
  
  static bool IsSigninProfile(Profile* profile);

  
  
  void ProfileStartup(Profile* profile, bool process_startup);

  
  base::FilePath GetActiveUserProfileDir();

  
  void Initialize();

  
  
  std::string active_user_id_hash() { return active_user_id_hash_; }

  
  
  void ClearSigninProfile(const base::Closure& on_clear_callback);

 private:
  friend class ProfileHelperTest;
  friend class ProfileListChromeOSTest;

  
  virtual void OnBrowsingDataRemoverDone() OVERRIDE;

  
  virtual void OnSessionRestoreStateChanged(
      Profile* user_profile,
      OAuth2LoginManager::SessionRestoreState state) OVERRIDE;

  
  virtual void ActiveUserHashChanged(const std::string& hash) OVERRIDE;

  
  std::string active_user_id_hash_;

  
  bool signin_profile_clear_requested_;

  
  std::vector<base::Closure> on_clear_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(ProfileHelper);
};

} 

#endif  
