// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_POLICY_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_POLICY_PROVIDER_H_


#include <vector>

#include "base/basictypes.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/content_settings/content_settings_observable_provider.h"
#include "chrome/browser/content_settings/content_settings_origin_identifier_value_map.h"

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace content_settings {

class PolicyProvider : public ObservableProvider {
 public:
  explicit PolicyProvider(PrefService* prefs);
  virtual ~PolicyProvider();
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  virtual RuleIterator* GetRuleIterator(
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      bool incognito) const OVERRIDE;

  virtual bool SetWebsiteSetting(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      Value* value) OVERRIDE;

  virtual void ClearAllContentSettingsRules(
      ContentSettingsType content_type) OVERRIDE;

  virtual void ShutdownOnUIThread() OVERRIDE;

 private:
  
  void ReadManagedDefaultSettings();

  
  void OnPreferenceChanged(const std::string& pref_name);

  
  void UpdateManagedDefaultSetting(ContentSettingsType content_type);

  void ReadManagedContentSettings(bool overwrite);

  void GetContentSettingsFromPreferences(OriginIdentifierValueMap* rules);

  void GetAutoSelectCertificateSettingsFromPreferences(
      OriginIdentifierValueMap* value_map);

  void ReadManagedContentSettingsTypes(ContentSettingsType content_type);

  OriginIdentifierValueMap value_map_;

  PrefService* prefs_;

  PrefChangeRegistrar pref_change_registrar_;

  
  
  mutable base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(PolicyProvider);
};

}  

#endif  
