// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_DEFAULT_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_DEFAULT_PROVIDER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/content_settings/content_settings_observable_provider.h"

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace content_settings {

class DefaultProvider : public ObservableProvider {
 public:
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  DefaultProvider(PrefService* prefs,
                  bool incognito);
  virtual ~DefaultProvider();

  
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
  
  void GetSettingsFromDictionary(const base::DictionaryValue* dictionary);

  
  
  void ForceDefaultsToBeExplicit();

  
  
  void ReadDefaultSettings(bool overwrite);

  
  void OnPreferenceChanged(const std::string& pref_name);

  typedef linked_ptr<base::Value> ValuePtr;
  typedef std::map<ContentSettingsType, ValuePtr> ValueMap;
  
  ValueMap default_settings_;

  PrefService* prefs_;

  
  bool is_incognito_;

  
  
  mutable base::Lock lock_;

  PrefChangeRegistrar pref_change_registrar_;

  
  
  bool updating_preferences_;

  DISALLOW_COPY_AND_ASSIGN(DefaultProvider);
};

}  

#endif  
