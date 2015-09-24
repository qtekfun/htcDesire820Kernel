// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_FACTORY_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace syncer {
class Invalidator;
}

class Profile;

namespace invalidation {

class InvalidationService;
class P2PInvalidationService;
class FakeInvalidationService;

class InvalidationServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static InvalidationService* GetForProfile(Profile* profile);

  static InvalidationServiceFactory* GetInstance();

  
  
  void SetBuildOnlyFakeInvalidatorsForTest(bool test_mode_enabled);

  
  
  P2PInvalidationService* BuildAndUseP2PInvalidationServiceForTest(
      content::BrowserContext* context);

 private:
  friend struct DefaultSingletonTraits<InvalidationServiceFactory>;

  InvalidationServiceFactory();
  virtual ~InvalidationServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;

  
  bool build_fake_invalidators_;

  DISALLOW_COPY_AND_ASSIGN(InvalidationServiceFactory);
};

}  

#endif  
