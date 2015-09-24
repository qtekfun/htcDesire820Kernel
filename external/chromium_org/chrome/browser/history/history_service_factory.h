// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_SERVICE_FACTORY_H_
#define CHROME_BROWSER_HISTORY_HISTORY_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class HistoryService;

class HistoryServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static HistoryService* GetForProfile(
      Profile* profile, Profile::ServiceAccessType sat);

  static HistoryService* GetForProfileIfExists(
      Profile* profile, Profile::ServiceAccessType sat);

  static HistoryService* GetForProfileWithoutCreating(
      Profile* profile);

  static HistoryServiceFactory* GetInstance();

  
  
  
  
  static void ShutdownForProfile(Profile* profile);

 private:
  friend struct DefaultSingletonTraits<HistoryServiceFactory>;

  HistoryServiceFactory();
  virtual ~HistoryServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
};

#endif  
