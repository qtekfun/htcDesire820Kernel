// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CONTENT_SETTINGS_H_
#define CHROME_COMMON_CONTENT_SETTINGS_H_

#include <string>
#include <vector>

#include "chrome/common/content_settings_pattern.h"
#include "chrome/common/content_settings_types.h"

enum ContentSetting {
  CONTENT_SETTING_DEFAULT = 0,
  CONTENT_SETTING_ALLOW,
  CONTENT_SETTING_BLOCK,
  CONTENT_SETTING_ASK,
  CONTENT_SETTING_SESSION_ONLY,
  CONTENT_SETTING_NUM_SETTINGS
};

ContentSetting IntToContentSetting(int content_setting);

struct ContentSettingPatternSource {
  ContentSettingPatternSource(const ContentSettingsPattern& primary_pattern,
                              const ContentSettingsPattern& secondary_patttern,
                              ContentSetting setting,
                              const std::string& source,
                              bool incognito);
  ContentSettingPatternSource();
  ContentSettingsPattern primary_pattern;
  ContentSettingsPattern secondary_pattern;
  ContentSetting setting;
  std::string source;
  bool incognito;
};

typedef std::vector<ContentSettingPatternSource> ContentSettingsForOneType;

struct RendererContentSettingRules {
  RendererContentSettingRules();
  ~RendererContentSettingRules();
  ContentSettingsForOneType image_rules;
  ContentSettingsForOneType script_rules;
};

namespace content_settings {

enum SettingSource {
  SETTING_SOURCE_NONE,
  SETTING_SOURCE_POLICY,
  SETTING_SOURCE_EXTENSION,
  SETTING_SOURCE_USER,
  SETTING_SOURCE_WHITELIST,
};

struct SettingInfo {
  SettingSource source;
  ContentSettingsPattern primary_pattern;
  ContentSettingsPattern secondary_pattern;
};

}  

#endif  
