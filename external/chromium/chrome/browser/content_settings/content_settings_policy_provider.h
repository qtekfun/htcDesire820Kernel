// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_POLICY_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_POLICY_PROVIDER_H_
#pragma once


#include <vector>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "base/tuple.h"
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

class PolicyDefaultProvider : public DefaultProviderInterface,
                              public NotificationObserver {
 public:
  explicit PolicyDefaultProvider(Profile* profile);
  virtual ~PolicyDefaultProvider();

  
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

  
  void ReadManagedDefaultSettings();

  
  void UpdateManagedDefaultSetting(ContentSettingsType content_type);

  
  ContentSettings managed_default_content_settings_;

  Profile* profile_;

  
  bool is_off_the_record_;

  
  
  mutable base::Lock lock_;

  PrefChangeRegistrar pref_change_registrar_;
  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(PolicyDefaultProvider);
};

class PolicyProvider : public BaseProvider,
                       public NotificationObserver {
 public:
  explicit PolicyProvider(Profile* profile);
  ~PolicyProvider();
  static void RegisterUserPrefs(PrefService* prefs);

  
  virtual void Init();

  
  virtual bool ContentSettingsTypeIsManaged(
      ContentSettingsType content_type);

  virtual void SetContentSetting(
      const ContentSettingsPattern& requesting_pattern,
      const ContentSettingsPattern& embedding_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      ContentSetting content_setting);

  virtual ContentSetting GetContentSetting(
      const GURL& requesting_url,
      const GURL& embedding_url,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier) const;

  virtual void ClearAllContentSettingsRules(
      ContentSettingsType content_type);

  virtual void ResetToDefaults();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);
 private:
  typedef Tuple5<
      ContentSettingsPattern,
      ContentSettingsPattern,
      ContentSettingsType,
      content_settings::ProviderInterface::ResourceIdentifier,
      ContentSetting> ContentSettingsRule;

  typedef std::vector<ContentSettingsRule> ContentSettingsRules;

  void ReadManagedContentSettings(bool overwrite);

  void GetContentSettingsFromPreferences(PrefService* prefs,
                                         ContentSettingsRules* rules);

  void ReadManagedContentSettingsTypes(
      ContentSettingsType content_type);

  void NotifyObservers(const ContentSettingsDetails& details);

  void UnregisterObservers();

  Profile* profile_;

  bool content_type_is_managed_[CONTENT_SETTINGS_NUM_TYPES];

  PrefChangeRegistrar pref_change_registrar_;
  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(PolicyProvider);
};

}  

#endif  
