// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_PERSONAL_DATA_MANAGER_FACTORY_H_
#define CHROME_BROWSER_AUTOFILL_PERSONAL_DATA_MANAGER_FACTORY_H_

#include "base/compiler_specific.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

template <typename T> struct DefaultSingletonTraits;
class Profile;

namespace autofill {

class PersonalDataManager;

class PersonalDataManagerService : public BrowserContextKeyedService {
 public:
  virtual PersonalDataManager* GetPersonalDataManager() = 0;
};

class PersonalDataManagerFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static PersonalDataManager* GetForProfile(Profile* profile);

  static PersonalDataManagerFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<PersonalDataManagerFactory>;

  PersonalDataManagerFactory();
  virtual ~PersonalDataManagerFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
};

}  

#endif  
