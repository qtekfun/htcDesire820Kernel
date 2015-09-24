// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PROVIDER_H_
#pragma once

#define NO_RESOURCE_IDENTIFIER ""

#include <string>
#include <vector>

#include "chrome/browser/content_settings/content_settings_pattern.h"
#include "chrome/common/content_settings.h"

class GURL;

namespace content_settings {

class DefaultProviderInterface {
 public:
  virtual ~DefaultProviderInterface() {}

  
  
  
  virtual ContentSetting ProvideDefaultSetting(
      ContentSettingsType content_type) const = 0;

  
  
  
  virtual void UpdateDefaultSetting(ContentSettingsType content_type,
                                    ContentSetting setting) = 0;

  
  virtual void ResetToDefaults() = 0;

  
  
  virtual bool DefaultSettingIsManaged(
      ContentSettingsType content_type) const = 0;
};

class ProviderInterface {
 public:
  typedef std::string ResourceIdentifier;

  struct Rule {
    Rule() {}
    Rule(const ContentSettingsPattern& requesting_pattern,
         const ContentSettingsPattern& embedding_pattern,
         ContentSetting setting)
      : requesting_url_pattern(requesting_pattern),
        embedding_url_pattern(embedding_pattern),
        content_setting(setting) {}

    ContentSettingsPattern requesting_url_pattern;
    ContentSettingsPattern embedding_url_pattern;
    ContentSetting content_setting;
  };

  typedef std::vector<Rule> Rules;

  virtual ~ProviderInterface() {}

  
  
  virtual bool ContentSettingsTypeIsManaged(
      ContentSettingsType content_type) = 0;

  
  
  
  
  
  
  virtual ContentSetting GetContentSetting(
      const GURL& requesting_url,
      const GURL& embedding_url,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier) const = 0;

  
  
  
  
  
  
  virtual void SetContentSetting(
      const ContentSettingsPattern& requesting_url_pattern,
      const ContentSettingsPattern& embedding_url_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      ContentSetting content_setting) = 0;

  
  
  
  
  
  
  
  
  
  virtual void GetAllContentSettingsRules(
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      Rules* content_setting_rules) const = 0;

  
  
  
  
  
  
  virtual void ClearAllContentSettingsRules(
      ContentSettingsType content_type) = 0;

  
  
  
  virtual void ResetToDefaults() = 0;
};

}  

#endif  
