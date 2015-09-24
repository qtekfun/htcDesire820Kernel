// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_GAIA_INFO_UPDATE_SERVICE_FACTORY_H_
#define CHROME_BROWSER_PROFILES_GAIA_INFO_UPDATE_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class GAIAInfoUpdateService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

class GAIAInfoUpdateServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  
  static GAIAInfoUpdateService* GetForProfile(Profile* profile);

  
  static GAIAInfoUpdateServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<GAIAInfoUpdateServiceFactory>;

  GAIAInfoUpdateServiceFactory();
  virtual ~GAIAInfoUpdateServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(GAIAInfoUpdateServiceFactory);
};

#endif  
