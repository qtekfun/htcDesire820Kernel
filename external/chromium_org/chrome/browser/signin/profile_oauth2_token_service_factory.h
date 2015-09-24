// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_PROFILE_OAUTH2_TOKEN_SERVICE_FACTORY_H_
#define CHROME_BROWSER_SIGNIN_PROFILE_OAUTH2_TOKEN_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class ProfileOAuth2TokenService;
class Profile;

#if defined(OS_ANDROID)
class AndroidProfileOAuth2TokenService;
#else
class MutableProfileOAuth2TokenService;
#endif

class ProfileOAuth2TokenServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  
  
  
  static ProfileOAuth2TokenService* GetForProfile(Profile* profile);

  
  
  
  
  #if defined(OS_ANDROID)
  static AndroidProfileOAuth2TokenService* GetPlatformSpecificForProfile(
      Profile* profile);
  #else
  static MutableProfileOAuth2TokenService* GetPlatformSpecificForProfile(
      Profile* profile);
  #endif

  
  static ProfileOAuth2TokenServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<ProfileOAuth2TokenServiceFactory>;

#if defined(OS_ANDROID)
  typedef AndroidProfileOAuth2TokenService PlatformSpecificOAuth2TokenService;
#else
  typedef MutableProfileOAuth2TokenService PlatformSpecificOAuth2TokenService;
#endif

  ProfileOAuth2TokenServiceFactory();
  virtual ~ProfileOAuth2TokenServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ProfileOAuth2TokenServiceFactory);
};

#endif  
