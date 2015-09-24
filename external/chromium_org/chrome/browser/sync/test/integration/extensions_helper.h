// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_EXTENSIONS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_EXTENSIONS_HELPER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/test/integration/sync_test.h"

class Profile;

namespace extensions_helper {

bool HasSameExtensionsAsVerifier(int index) WARN_UNUSED_RESULT;

bool AllProfilesHaveSameExtensionsAsVerifier() WARN_UNUSED_RESULT;

bool AllProfilesHaveSameExtensions() WARN_UNUSED_RESULT;

std::string InstallExtension(Profile* profile, int index);

std::string InstallExtensionForAllProfiles(int index);

void UninstallExtension(Profile* profile, int index);

std::vector<int> GetInstalledExtensions(Profile* profile);

void InstallExtensionsPendingForSync(Profile* profile);

void EnableExtension(Profile* profile, int index);

void DisableExtension(Profile* profile, int index);

bool IsExtensionEnabled(Profile* profile, int index);

void IncognitoEnableExtension(Profile* profile, int index);

void IncognitoDisableExtension(Profile* profile, int index);

bool IsIncognitoEnabled(Profile* profile, int index);

std::string CreateFakeExtensionName(int index);

bool ExtensionNameToIndex(const std::string& name, int* index);

}  

#endif  
