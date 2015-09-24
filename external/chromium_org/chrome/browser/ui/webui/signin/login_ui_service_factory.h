// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SIGNIN_LOGIN_UI_SERVICE_FACTORY_H_
#define CHROME_BROWSER_UI_WEBUI_SIGNIN_LOGIN_UI_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class LoginUIService;
class Profile;

class LoginUIServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  
  static LoginUIService* GetForProfile(Profile* profile);

  
  static LoginUIServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<LoginUIServiceFactory>;

  LoginUIServiceFactory();
  virtual ~LoginUIServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(LoginUIServiceFactory);
};

#endif  
