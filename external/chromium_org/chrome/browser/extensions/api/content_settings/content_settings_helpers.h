// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_CONTENT_SETTINGS_CONTENT_SETTINGS_HELPERS_H__
#define CHROME_BROWSER_EXTENSIONS_API_CONTENT_SETTINGS_CONTENT_SETTINGS_HELPERS_H__

#include <string>

#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_pattern.h"

namespace extensions {
namespace content_settings_helpers {

ContentSettingsPattern ParseExtensionPattern(const std::string& pattern_str,
                                             std::string* error);

ContentSettingsType StringToContentSettingsType(
    const std::string& content_type);
const char* ContentSettingsTypeToString(ContentSettingsType type);

bool StringToContentSetting(const std::string& setting_str,
                            ContentSetting* setting);
const char* ContentSettingToString(ContentSetting setting);

}  
}  

#endif  
