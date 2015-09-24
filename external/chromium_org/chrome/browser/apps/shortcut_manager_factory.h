// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APPS_SHORTCUT_MANAGER_FACTORY_H_
#define CHROME_BROWSER_APPS_SHORTCUT_MANAGER_FACTORY_H_

#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

template<typename Type> struct DefaultSingletonTraits;

class Profile;

class AppShortcutManager;

class AppShortcutManagerFactory : public BrowserContextKeyedServiceFactory {
 public:
  static AppShortcutManager* GetForProfile(Profile* profile);

  static AppShortcutManagerFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<AppShortcutManagerFactory>;

  AppShortcutManagerFactory();
  virtual ~AppShortcutManagerFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
};

#endif  
