// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_COOKIE_SETTINGS_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_COOKIE_SETTINGS_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/content_settings/host_content_settings_map.h"
#include "chrome/common/content_settings.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service_factory.h"

class ContentSettingsPattern;
class CookieSettingsWrapper;
class GURL;
class PrefService;
class Profile;

// cookie-specific logic such as blocking third-party cookies. Written on the UI
class CookieSettings : public RefcountedBrowserContextKeyedService {
 public:
  CookieSettings(
      HostContentSettingsMap* host_content_settings_map,
      PrefService* prefs);

  
  
  
  
  
  
  ContentSetting GetDefaultCookieSetting(std::string* provider_id) const;

  
  
  
  
  bool IsReadingCookieAllowed(const GURL& url,
                              const GURL& first_party_url) const;

  
  
  
  
  bool IsSettingCookieAllowed(const GURL& url,
                              const GURL& first_party_url) const;

  
  
  
  
  
  bool IsCookieSessionOnly(const GURL& url) const;

  
  
  
  
  
  void GetCookieSettings(ContentSettingsForOneType* settings) const;

  
  
  
  
  void SetDefaultCookieSetting(ContentSetting setting);

  
  
  
  void SetCookieSetting(const ContentSettingsPattern& primary_pattern,
                        const ContentSettingsPattern& secondary_pattern,
                        ContentSetting setting);

  
  
  
  void ResetCookieSetting(const ContentSettingsPattern& primary_pattern,
                          const ContentSettingsPattern& secondary_pattern);

  
  
  
  virtual void ShutdownOnUIThread() OVERRIDE;

  
  ContentSetting GetCookieSetting(
      const GURL& url,
      const GURL& first_party_url,
      bool setting_cookie,
      content_settings::SettingSource* source) const;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  class Factory : public RefcountedBrowserContextKeyedServiceFactory {
   public:
    
    
    
    static scoped_refptr<CookieSettings> GetForProfile(Profile* profile);

    static Factory* GetInstance();

   private:
    friend struct DefaultSingletonTraits<Factory>;

    Factory();
    virtual ~Factory();

    
    virtual void RegisterProfilePrefs(
        user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
    virtual content::BrowserContext* GetBrowserContextToUse(
        content::BrowserContext* context) const OVERRIDE;
    virtual scoped_refptr<RefcountedBrowserContextKeyedService>
        BuildServiceInstanceFor(
            content::BrowserContext* context) const OVERRIDE;
  };

 private:
  virtual ~CookieSettings();

  void OnBlockThirdPartyCookiesChanged();

  
  
  
  bool ShouldBlockThirdPartyCookies() const;

  scoped_refptr<HostContentSettingsMap> host_content_settings_map_;
  PrefChangeRegistrar pref_change_registrar_;

  
  
  mutable base::Lock lock_;

  bool block_third_party_cookies_;
};

#endif  
