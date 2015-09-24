// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Maps [requesting_origin, embedder] to content settings.  Written on the UI

#ifndef CHROME_BROWSER_GEOLOCATION_GEOLOCATION_CONTENT_SETTINGS_MAP_H_
#define CHROME_BROWSER_GEOLOCATION_GEOLOCATION_CONTENT_SETTINGS_MAP_H_
#pragma once

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/common/content_settings.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"

class ContentSettingsDetails;
class DictionaryValue;
class PrefService;
class Profile;

class GeolocationContentSettingsMap
    : public base::RefCountedThreadSafe<GeolocationContentSettingsMap>,
      public NotificationObserver {
 public:
  typedef std::map<GURL, ContentSetting> OneOriginSettings;
  typedef std::map<GURL, OneOriginSettings> AllOriginsSettings;

  explicit GeolocationContentSettingsMap(Profile* profile);

  virtual ~GeolocationContentSettingsMap();

  static void RegisterUserPrefs(PrefService* prefs);

  
  
  
  ContentSetting GetDefaultContentSetting() const;

  
  bool IsDefaultContentSettingManaged() const;

  
  
  
  
  
  
  
  ContentSetting GetContentSetting(const GURL& requesting_url,
                                   const GURL& embedding_url) const;

  
  
  
  AllOriginsSettings GetAllOriginsSettings() const;

  
  
  
  void SetDefaultContentSetting(ContentSetting setting);

  
  
  
  
  
  
  
  
  
  
  
  
  void SetContentSetting(const GURL& requesting_url,
                         const GURL& embedding_url,
                         ContentSetting setting);

  
  
  
  void ResetToDefault();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  friend class base::RefCountedThreadSafe<GeolocationContentSettingsMap>;

  
  static const ContentSetting kDefaultSetting;

  
  void NotifyObservers(const ContentSettingsDetails& details);

  void UnregisterObservers();

  
  
  static void GetOneOriginSettingsFromDictionary(
      const DictionaryValue* dictionary,
      OneOriginSettings* one_origin_settings);

  
  Profile* profile_;

  
  PrefChangeRegistrar prefs_registrar_;
  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(GeolocationContentSettingsMap);
};

#endif  
