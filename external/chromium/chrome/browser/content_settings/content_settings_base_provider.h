// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_BASE_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_BASE_PROVIDER_H_
#pragma once

#include <map>
#include <string>
#include <utility>

#include "base/synchronization/lock.h"
#include "chrome/browser/content_settings/content_settings_provider.h"

namespace content_settings {

typedef std::pair<ContentSettingsType, std::string>
    ContentSettingsTypeResourceIdentifierPair;
typedef std::map<ContentSettingsTypeResourceIdentifierPair, ContentSetting>
    ResourceContentSettings;

struct ExtendedContentSettings {
  ExtendedContentSettings();
  ExtendedContentSettings(const ExtendedContentSettings& rhs);
  ~ExtendedContentSettings();

  ContentSettings content_settings;
  ResourceContentSettings content_settings_for_resources;
};

typedef std::map<std::string, ExtendedContentSettings> HostContentSettings;

class BaseProvider : public ProviderInterface {
 public:
  
  
  static ContentSetting ClickToPlayFixup(ContentSettingsType content_type,
                                         ContentSetting setting);

  explicit BaseProvider(bool is_otr);
  virtual ~BaseProvider();


  
  virtual void Init() = 0;

  
  virtual bool ContentSettingsTypeIsManaged(
      ContentSettingsType content_type) = 0;

  virtual ContentSetting GetContentSetting(
      const GURL& requesting_url,
      const GURL& embedding_url,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier) const;

  virtual void SetContentSetting(
      const ContentSettingsPattern& requesting_pattern,
      const ContentSettingsPattern& embedding_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      ContentSetting content_setting) = 0;

  virtual void GetAllContentSettingsRules(
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      Rules* content_setting_rules) const;

  virtual void ClearAllContentSettingsRules(
      ContentSettingsType content_type) = 0;

  virtual void ResetToDefaults() = 0;

 protected:
  
  bool RequiresResourceIdentifier(
      ContentSettingsType content_type) const;

  
  
  bool AllDefault(const ExtendedContentSettings& settings) const;

  void UpdateContentSettingsMap(
      const ContentSettingsPattern& requesting_pattern,
      const ContentSettingsPattern& embedding_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      ContentSetting content_setting);

  
  
  ContentSettings GetNonDefaultContentSettings(const GURL& url) const;

  
  HostContentSettings* host_content_settings() {
    return &host_content_settings_;
  }

  HostContentSettings* incognito_settings() {
    return &incognito_settings_;
  }

  base::Lock& lock() const {
    return lock_;
  }

  bool is_incognito() const {
    return is_incognito_;
  }

 private:
  
  
  HostContentSettings host_content_settings_;

  
  bool is_incognito_;

  
  
  HostContentSettings incognito_settings_;

  
  
  mutable base::Lock lock_;
};

}  

#endif  
