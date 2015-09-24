// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_FACTORY_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service_factory.h"

class PasswordStore;
class Profile;

#if !defined(OS_MACOSX) && !defined(OS_CHROMEOS) && defined(OS_POSIX)
typedef int LocalProfileId;
#endif

class PasswordStoreFactory
    : public RefcountedBrowserContextKeyedServiceFactory {
 public:
  static scoped_refptr<PasswordStore> GetForProfile(
      Profile* profile, Profile::ServiceAccessType set);

  static PasswordStoreFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<PasswordStoreFactory>;

  PasswordStoreFactory();
  virtual ~PasswordStoreFactory();

#if !defined(OS_MACOSX) && !defined(OS_CHROMEOS) && defined(OS_POSIX)
  LocalProfileId GetLocalProfileId(PrefService* prefs) const;
#endif

  
  virtual scoped_refptr<RefcountedBrowserContextKeyedService>
      BuildServiceInstanceFor(content::BrowserContext* context) const OVERRIDE;
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreFactory);
};

#endif  
