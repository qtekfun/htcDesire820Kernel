// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_EXTENSION_SETTINGS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_EXTENSION_SETTINGS_HELPER_H_

#include <string>

class Profile;
namespace base {
class DictionaryValue;
}

namespace extension_settings_helper {

void SetExtensionSettings(
    Profile* profile,
    const std::string& id,
    const base::DictionaryValue& settings);

void SetExtensionSettingsForAllProfiles(
    const std::string& id, const base::DictionaryValue& settings);

bool AllExtensionSettingsSameAsVerifier();

}  

#endif  
