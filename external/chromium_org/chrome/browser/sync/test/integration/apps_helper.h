// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_APPS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_APPS_HELPER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/test/integration/sync_test.h"
#include "sync/api/string_ordinal.h"

class Profile;

namespace apps_helper {

bool HasSameAppsAsVerifier(int index) WARN_UNUSED_RESULT;

bool AllProfilesHaveSameAppsAsVerifier() WARN_UNUSED_RESULT;

std::string InstallApp(Profile* profile, int index);

std::string InstallPlatformApp(Profile* profile, int index);

std::string InstallAppForAllProfiles(int index);

void UninstallApp(Profile* profile, int index);

void InstallAppsPendingForSync(Profile* profile);

void EnableApp(Profile* profile, int index);

void DisableApp(Profile* profile, int index);

void IncognitoEnableApp(Profile* profile, int index);

void IncognitoDisableApp(Profile* profile, int index);

syncer::StringOrdinal GetPageOrdinalForApp(Profile* profile, int app_index);

void SetPageOrdinalForApp(
    Profile* profile, int app_index,
    const syncer::StringOrdinal& page_ordinal);

syncer::StringOrdinal GetAppLaunchOrdinalForApp(
    Profile* profile, int app_index);

void SetAppLaunchOrdinalForApp(
    Profile* profile, int app_index,
    const syncer::StringOrdinal& app_launch_ordinal);

void CopyNTPOrdinals(Profile* source, Profile* destination, int index);

void FixNTPOrdinalCollisions(Profile* profile);

}  

#endif  
