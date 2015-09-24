// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_ACCOUNT_RECONCILOR_FACTORY_H_
#define CHROME_BROWSER_SIGNIN_ACCOUNT_RECONCILOR_FACTORY_H_

#include "base/memory/singleton.h"
#include "chrome/browser/signin/account_reconcilor.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class AccountReconcilor;
class Profile;

class AccountReconcilorFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  
  static AccountReconcilor* GetForProfile(Profile* profile);

  
  static AccountReconcilorFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<AccountReconcilorFactory>;

  AccountReconcilorFactory();
  virtual ~AccountReconcilorFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
};

#endif  
