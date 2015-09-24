// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OAUTH2_LOGIN_MANAGER_FACTORY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OAUTH2_LOGIN_MANAGER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class Profile;

namespace chromeos {

class OAuth2LoginManager;

class OAuth2LoginManagerFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static OAuth2LoginManager* GetForProfile(Profile* profile);

  
  static OAuth2LoginManagerFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<OAuth2LoginManagerFactory>;

  OAuth2LoginManagerFactory();
  virtual ~OAuth2LoginManagerFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(OAuth2LoginManagerFactory);
};

}  

#endif  
