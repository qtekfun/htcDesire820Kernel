// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_SYNC_OBSERVER_FACTORY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_SYNC_OBSERVER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class Profile;

namespace chromeos {

class AuthSyncObserver;

class AuthSyncObserverFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static AuthSyncObserver* GetForProfile(Profile* profile);

  
  static AuthSyncObserverFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<AuthSyncObserverFactory>;

  AuthSyncObserverFactory();
  virtual ~AuthSyncObserverFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(AuthSyncObserverFactory);
};

}  

#endif  
