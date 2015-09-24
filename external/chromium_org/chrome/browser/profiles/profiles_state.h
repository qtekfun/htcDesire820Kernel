// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILES_STATE_H_
#define CHROME_BROWSER_PROFILES_PROFILES_STATE_H_

#include "base/strings/string16.h"

class Browser;
class PrefRegistrySimple;
class Profile;
namespace base { class FilePath; }

namespace profiles {

bool IsMultipleProfilesEnabled();

bool IsNewProfileManagementEnabled();

base::FilePath GetDefaultProfileDir(const base::FilePath& user_data_dir);

base::FilePath GetProfilePrefsPath(const base::FilePath& profile_dir);

void RegisterPrefs(PrefRegistrySimple* registry);

base::string16 GetActiveProfileDisplayName(Browser* browser);

void UpdateProfileName(Profile* profile,
                       const base::string16& new_profile_name);

}  

#endif  
