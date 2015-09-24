// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_INSTANT_SERVICE_FACTORY_H_
#define CHROME_BROWSER_SEARCH_INSTANT_SERVICE_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class InstantService;
class Profile;
class BrowserContextKeyedService;

class InstantServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  static InstantService* GetForProfile(Profile* profile);

  static InstantServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<InstantServiceFactory>;

  InstantServiceFactory();
  virtual ~InstantServiceFactory();

  
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(InstantServiceFactory);
};

#endif  
