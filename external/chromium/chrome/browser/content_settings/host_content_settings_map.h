// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Maps hostnames to custom content settings.  Written on the UI thread and read

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_HOST_CONTENT_SETTINGS_MAP_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_HOST_CONTENT_SETTINGS_MAP_H_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/content_settings/content_settings_pattern.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/common/content_settings.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace content_settings {
class DefaultProviderInterface;
class ProviderInterface;
}  

class ContentSettingsDetails;
class DictionaryValue;
class GURL;
class PrefService;
class Profile;

class HostContentSettingsMap
    : public NotificationObserver,
      public base::RefCountedThreadSafe<HostContentSettingsMap,
                                        BrowserThread::DeleteOnUIThread> {
 public:
  typedef std::pair<ContentSettingsPattern, ContentSetting> PatternSettingPair;
  typedef std::vector<PatternSettingPair> SettingsForOneType;

  explicit HostContentSettingsMap(Profile* profile);

  static void RegisterUserPrefs(PrefService* prefs);

  
  
  
  ContentSetting GetDefaultContentSetting(
      ContentSettingsType content_type) const;

  
  
  
  
  
  
  ContentSetting GetContentSetting(
      const GURL& url,
      ContentSettingsType content_type,
      const std::string& resource_identifier) const;

  
  
  
  
  
  
  
  ContentSetting GetNonDefaultContentSetting(
      const GURL& url,
      ContentSettingsType content_type,
      const std::string& resource_identifier) const;

  
  
  
  
  
  ContentSettings GetContentSettings(const GURL& url) const;

  
  
  
  
  
  ContentSettings GetNonDefaultContentSettings(const GURL& url) const;

  
  
  
  
  
  
  
  
  void GetSettingsForOneType(ContentSettingsType content_type,
                             const std::string& resource_identifier,
                             SettingsForOneType* settings) const;

  
  
  
  
  void SetDefaultContentSetting(ContentSettingsType content_type,
                                ContentSetting setting);

  
  
  
  
  
  
  
  void SetContentSetting(const ContentSettingsPattern& pattern,
                         ContentSettingsType content_type,
                         const std::string& resource_identifier,
                         ContentSetting setting);

  
  
  
  
  
  
  void AddExceptionForURL(const GURL& url,
                          ContentSettingsType content_type,
                          const std::string& resource_identifier,
                          ContentSetting setting);

  
  
  
  void ClearSettingsForOneType(ContentSettingsType content_type);

  
  bool BlockThirdPartyCookies() const { return block_third_party_cookies_; }
  bool IsBlockThirdPartyCookiesManaged() const {
    return is_block_third_party_cookies_managed_;
  }

  
  
  
  
  void SetBlockThirdPartyCookies(bool block);

  bool GetBlockNonsandboxedPlugins() const {
    return block_nonsandboxed_plugins_;
  }

  void SetBlockNonsandboxedPlugins(bool block);

  
  
  
  void ResetToDefaults();

  
  bool IsDefaultContentSettingManaged(ContentSettingsType content_type) const;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<HostContentSettingsMap>;

  ~HostContentSettingsMap();

  
  
  
  
  void NotifyObservers(const ContentSettingsDetails& details);

  void UnregisterObservers();

  
  
  void MigrateObsoleteCookiePref(PrefService* prefs);

  
  Profile* profile_;

  NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  
  bool is_off_the_record_;

  
  
  bool updating_preferences_;

  
  std::vector<linked_ptr<content_settings::DefaultProviderInterface> >
      default_content_settings_providers_;

  
  std::vector<linked_ptr<content_settings::ProviderInterface> >
      content_settings_providers_;

  
  mutable base::Lock lock_;

  
  bool block_third_party_cookies_;
  bool is_block_third_party_cookies_managed_;
  bool block_nonsandboxed_plugins_;

  DISALLOW_COPY_AND_ASSIGN(HostContentSettingsMap);
};

#endif  
