// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_THEMES_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_THEMES_HELPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/test/integration/sync_test.h"

class Profile;

namespace themes_helper {

std::string GetCustomTheme(int index) WARN_UNUSED_RESULT;

std::string GetThemeID(Profile* profile) WARN_UNUSED_RESULT;

bool UsingCustomTheme(Profile* profile) WARN_UNUSED_RESULT;

bool UsingDefaultTheme(Profile* profile) WARN_UNUSED_RESULT;

bool UsingNativeTheme(Profile* profile) WARN_UNUSED_RESULT;

bool ThemeIsPendingInstall(
    Profile* profile, const std::string& id) WARN_UNUSED_RESULT;

bool HasOrWillHaveCustomTheme(
    Profile* profile, const std::string& id) WARN_UNUSED_RESULT;

void UseCustomTheme(Profile* profile, int index);

void UseDefaultTheme(Profile* profile);

void UseNativeTheme(Profile* profile);

}  

#endif  
