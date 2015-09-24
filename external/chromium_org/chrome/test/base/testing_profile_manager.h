// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_TESTING_PROFILE_MANAGER_H_
#define CHROME_TEST_BASE_TESTING_PROFILE_MANAGER_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_profile.h"

class PrefServiceSyncable;
class ProfileInfoCache;
class ProfileManager;
class TestingBrowserProcess;
class TestingProfile;

class TestingProfileManager {
 public:
  explicit TestingProfileManager(TestingBrowserProcess* browser_process);
  ~TestingProfileManager();

  
  
  
  
  bool SetUp() WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  
  TestingProfile* CreateTestingProfile(
      const std::string& profile_name,
      scoped_ptr<PrefServiceSyncable> prefs,
      const string16& user_name,
      int avatar_id,
      const std::string& managed_user_id,
      const TestingProfile::TestingFactories& factories);

  
  TestingProfile* CreateTestingProfile(const std::string& name);

  
  void DeleteTestingProfile(const std::string& profile_name);

  
  
  void DeleteProfileInfoCache();

  
  void SetLoggedIn(bool logged_in);

  
  const base::FilePath& profiles_dir();
  ProfileManager* profile_manager();
  ProfileInfoCache* profile_info_cache();

 private:
  typedef std::map<std::string, TestingProfile*> TestingProfilesMap;

  
  
  
  void SetUpInternal();

  
  bool called_set_up_;

  
  base::ScopedTempDir profiles_dir_;

  
  TestingBrowserProcess* browser_process_;

  
  ScopedTestingLocalState local_state_;

  
  ProfileManager* profile_manager_;

  
  TestingProfilesMap testing_profiles_;

  DISALLOW_COPY_AND_ASSIGN(TestingProfileManager);
};

#endif  
