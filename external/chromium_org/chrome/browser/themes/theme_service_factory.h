// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THEMES_THEME_SERVICE_FACTORY_H_
#define CHROME_BROWSER_THEMES_THEME_SERVICE_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class Profile;
class ThemeService;

namespace extensions {
class Extension;
}

class ThemeServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  
  static ThemeService* GetForProfile(Profile* profile);

  
  
  
  static const extensions::Extension* GetThemeForProfile(Profile* profile);

  static ThemeServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<ThemeServiceFactory>;

  ThemeServiceFactory();
  virtual ~ThemeServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ThemeServiceFactory);
};

#endif  
