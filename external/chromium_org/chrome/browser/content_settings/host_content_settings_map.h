// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Maps hostnames to custom content settings.  Written on the UI thread and read

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_HOST_CONTENT_SETTINGS_MAP_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_HOST_CONTENT_SETTINGS_MAP_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/threading/platform_thread.h"
#include "base/tuple.h"
#include "chrome/browser/content_settings/content_settings_observer.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_pattern.h"
#include "chrome/common/content_settings_types.h"

class ExtensionService;
class GURL;
class PrefService;

namespace base {
class Value;
}

namespace content_settings {
class ProviderInterface;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class HostContentSettingsMap
    : public content_settings::Observer,
      public base::RefCountedThreadSafe<HostContentSettingsMap> {
 public:
  enum ProviderType {
    INTERNAL_EXTENSION_PROVIDER = 0,
    POLICY_PROVIDER,
    CUSTOM_EXTENSION_PROVIDER,
    PREF_PROVIDER,
    DEFAULT_PROVIDER,
    NUM_PROVIDER_TYPES,
  };

  HostContentSettingsMap(PrefService* prefs, bool incognito);

#if defined(ENABLE_EXTENSIONS)
  
  
  
  void RegisterExtensionService(ExtensionService* extension_service);
#endif

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  
  
  ContentSetting GetDefaultContentSetting(ContentSettingsType content_type,
                                          std::string* provider_id) const;

  
  
  
  
  
  
  
  ContentSetting GetContentSetting(
      const GURL& primary_url,
      const GURL& secondary_url,
      ContentSettingsType content_type,
      const std::string& resource_identifier) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  base::Value* GetWebsiteSetting(
      const GURL& primary_url,
      const GURL& secondary_url,
      ContentSettingsType content_type,
      const std::string& resource_identifier,
      content_settings::SettingInfo* info) const;

  
  
  
  
  
  void GetSettingsForOneType(ContentSettingsType content_type,
                             const std::string& resource_identifier,
                             ContentSettingsForOneType* settings) const;

  
  
  
  
  void SetDefaultContentSetting(ContentSettingsType content_type,
                                ContentSetting setting);

  
  
  
  
  
  
  
  
  
  void SetContentSetting(const ContentSettingsPattern& primary_pattern,
                         const ContentSettingsPattern& secondary_pattern,
                         ContentSettingsType content_type,
                         const std::string& resource_identifier,
                         ContentSetting setting);

  
  
  
  
  
  void SetWebsiteSetting(const ContentSettingsPattern& primary_pattern,
                         const ContentSettingsPattern& secondary_pattern,
                         ContentSettingsType content_type,
                         const std::string& resource_identifier,
                         base::Value* value);

  
  
  
  
  void AddExceptionForURL(const GURL& primary_url,
                          const GURL& secondary_url,
                          ContentSettingsType content_type,
                          ContentSetting setting);

  
  
  
  void ClearSettingsForOneType(ContentSettingsType content_type);

  static bool IsValueAllowedForType(PrefService* prefs,
                                    const base::Value* value,
                                    ContentSettingsType content_type);
  static bool IsSettingAllowedForType(PrefService* prefs,
                                      ContentSetting setting,
                                      ContentSettingsType content_type);

  
  static bool ContentTypeHasCompoundValue(ContentSettingsType type);

  
  
  
  
  void ShutdownOnUIThread();

  
  virtual void OnContentSettingChanged(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsType content_type,
      std::string resource_identifier) OVERRIDE;

  
  
  
  static bool ShouldAllowAllContent(const GURL& primary_url,
                                    const GURL& secondary_url,
                                    ContentSettingsType content_type);

  
  
  
  
  static ProviderType GetProviderTypeFromSource(const std::string& source);

  bool is_off_the_record() const {
    return is_off_the_record_;
  }

 private:
  friend class base::RefCountedThreadSafe<HostContentSettingsMap>;
  friend class HostContentSettingsMapTest_NonDefaultSettings_Test;

  typedef std::map<ProviderType, content_settings::ProviderInterface*>
      ProviderMap;
  typedef ProviderMap::iterator ProviderIterator;
  typedef ProviderMap::const_iterator ConstProviderIterator;

  virtual ~HostContentSettingsMap();

  ContentSetting GetDefaultContentSettingFromProvider(
      ContentSettingsType content_type,
      content_settings::ProviderInterface* provider) const;

  
  void MigrateObsoleteClearOnExitPref();

  
  
  
  
  
  void AddSettingsForOneType(
      const content_settings::ProviderInterface* provider,
      ProviderType provider_type,
      ContentSettingsType content_type,
      const std::string& resource_identifier,
      ContentSettingsForOneType* settings,
      bool incognito) const;

  
  
  
  
  void UsedContentSettingsProviders() const;

#ifndef NDEBUG
  
  
  
  
  
  
  mutable base::PlatformThreadId used_from_thread_id_;
#endif

  
  PrefService* prefs_;

  
  bool is_off_the_record_;

  
  
  
  ProviderMap content_settings_providers_;

  DISALLOW_COPY_AND_ASSIGN(HostContentSettingsMap);
};

#endif  
