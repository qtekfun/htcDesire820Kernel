// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PREF_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PREF_PROVIDER_H_
#pragma once


#include <map>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/content_settings/content_settings_base_provider.h"
#include "chrome/browser/content_settings/content_settings_provider.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ContentSettingsDetails;
class DictionaryValue;
class PrefService;
class Profile;

namespace content_settings {

class PrefDefaultProvider : public DefaultProviderInterface,
                            public NotificationObserver {
 public:
  explicit PrefDefaultProvider(Profile* profile);
  virtual ~PrefDefaultProvider();

  
  virtual ContentSetting ProvideDefaultSetting(
      ContentSettingsType content_type) const;
  virtual void UpdateDefaultSetting(ContentSettingsType content_type,
                                    ContentSetting setting);
  virtual void ResetToDefaults();
  virtual bool DefaultSettingIsManaged(ContentSettingsType content_type) const;

  static void RegisterUserPrefs(PrefService* prefs);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  
  
  
  void NotifyObservers(const ContentSettingsDetails& details);

  void UnregisterObservers();

  
  void GetSettingsFromDictionary(const DictionaryValue* dictionary,
                                 ContentSettings* settings);

  
  
  void ForceDefaultsToBeExplicit();

  
  
  void ReadDefaultSettings(bool overwrite);

  void MigrateObsoleteNotificationPref(PrefService* prefs);

  
  ContentSettings default_content_settings_;

  Profile* profile_;

  
  bool is_incognito_;

  
  
  mutable base::Lock lock_;

  PrefChangeRegistrar pref_change_registrar_;
  NotificationRegistrar notification_registrar_;

  
  
  bool updating_preferences_;

  bool initializing_;

  DISALLOW_COPY_AND_ASSIGN(PrefDefaultProvider);
};

class PrefProvider : public BaseProvider,
                     public NotificationObserver {
 public:
  static void RegisterUserPrefs(PrefService* prefs);

  explicit PrefProvider(Profile* profile);
  virtual ~PrefProvider();

  
  virtual bool ContentSettingsTypeIsManaged(
      ContentSettingsType content_type);

  virtual void SetContentSetting(
      const ContentSettingsPattern& requesting_pattern,
      const ContentSettingsPattern& embedding_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      ContentSetting content_setting);

  virtual void ClearAllContentSettingsRules(
      ContentSettingsType content_type);

  virtual void ResetToDefaults();

  
  virtual void Init();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  void ReadExceptions(bool overwrite);

  
  
  void MigrateObsoletePerhostPref(PrefService* prefs);
  void MigrateObsoletePopupsPref(PrefService* prefs);

  void CanonicalizeContentSettingsExceptions(
      DictionaryValue* all_settings_dictionary);

  void GetSettingsFromDictionary(
      const DictionaryValue* dictionary,
      ContentSettings* settings);

  void GetResourceSettingsFromDictionary(
      const DictionaryValue* dictionary,
      ResourceContentSettings* settings);

  void NotifyObservers(const ContentSettingsDetails& details);

  void UnregisterObservers();

  Profile* profile_;

  PrefChangeRegistrar pref_change_registrar_;
  NotificationRegistrar notification_registrar_;

  
  
  bool updating_preferences_;

  
  bool initializing_;

  DISALLOW_COPY_AND_ASSIGN(PrefProvider);
};

}  

#endif  
