// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_BROWSER_PREFS_H_
#define CHROME_BROWSER_PREFS_BROWSER_PREFS_H_

class PrefRegistrySimple;
class PrefService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome {

void RegisterLocalState(PrefRegistrySimple* registry);

void RegisterUserProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

#if defined(OS_CHROMEOS)
void RegisterLoginProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
#endif

void MigrateBrowserPrefs(Profile* profile, PrefService* local_state);

void MigrateUserPrefs(Profile* profile);

}  

#endif  
