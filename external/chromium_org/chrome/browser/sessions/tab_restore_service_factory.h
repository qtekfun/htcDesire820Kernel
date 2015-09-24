// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_FACTORY_H_
#define CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class TabRestoreService;
class Profile;

class TabRestoreServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static TabRestoreService* GetForProfile(Profile* profile);

  
  
  static TabRestoreService* GetForProfileIfExisting(Profile* profile);

  static void ResetForProfile(Profile* profile);

  static TabRestoreServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<TabRestoreServiceFactory>;

  TabRestoreServiceFactory();
  virtual ~TabRestoreServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
};

#endif  
