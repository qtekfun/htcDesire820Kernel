// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_ABOUT_SIGNIN_INTERNALS_FACTORY_H_
#define CHROME_BROWSER_SIGNIN_ABOUT_SIGNIN_INTERNALS_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class AboutSigninInternals;
class Profile;

class AboutSigninInternalsFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static AboutSigninInternals* GetForProfile(Profile* profile);

  
  static AboutSigninInternalsFactory* GetInstance();

  
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<AboutSigninInternalsFactory>;

  AboutSigninInternalsFactory();
  virtual ~AboutSigninInternalsFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
};

#endif  
