// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PROFILE_KEYED_API_FACTORY_H_
#define CHROME_BROWSER_EXTENSIONS_API_PROFILE_KEYED_API_FACTORY_H_

#include "chrome/browser/extensions/extension_system_factory.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/browser_context_dependency_manager.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace extensions {

template <typename T>
class ProfileKeyedAPIFactory;

class ProfileKeyedAPI : public BrowserContextKeyedService {
 protected:
  
  
  static const bool kServiceRedirectedInIncognito = false;
  static const bool kServiceIsNULLWhileTesting = false;
  static const bool kServiceHasOwnInstanceInIncognito = false;
  static const bool kServiceIsCreatedWithBrowserContext = true;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
};

template <typename T>
class ProfileKeyedAPIFactory : public BrowserContextKeyedServiceFactory {
 public:
  static T* GetForProfile(Profile* profile) {
    return static_cast<T*>(
        T::GetFactoryInstance()->GetServiceForBrowserContext(profile, true));
  }

  
  
  
  
  
  
  
  
  
  
  void DeclareFactoryDependencies() {
    DependsOn(ExtensionSystemFactory::GetInstance());
  }

  ProfileKeyedAPIFactory()
      : BrowserContextKeyedServiceFactory(
          T::service_name(),
          BrowserContextDependencyManager::GetInstance()) {
    DeclareFactoryDependencies();
  }

  virtual ~ProfileKeyedAPIFactory() {
  }

 private:
  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE {
    return new T(static_cast<Profile*>(profile));
  }

  
  
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE {
    if (T::kServiceRedirectedInIncognito)
      return chrome::GetBrowserContextRedirectedInIncognito(context);

    if (T::kServiceHasOwnInstanceInIncognito)
      return chrome::GetBrowserContextOwnInstanceInIncognito(context);

    return BrowserContextKeyedServiceFactory::GetBrowserContextToUse(context);
  }

  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE {
    return T::kServiceIsCreatedWithBrowserContext;
  }

  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE {
    return T::kServiceIsNULLWhileTesting;
  }

  DISALLOW_COPY_AND_ASSIGN(ProfileKeyedAPIFactory);
};

}  

#endif  
