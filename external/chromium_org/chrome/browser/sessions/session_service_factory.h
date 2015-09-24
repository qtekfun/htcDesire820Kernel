// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_SERVICE_FACTORY_H_
#define CHROME_BROWSER_SESSIONS_SESSION_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/session_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class SessionServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  
  
  
  
  
  
  static SessionService* GetForProfile(Profile* profile);

  
  
  
  
  static SessionService* GetForProfileIfExisting(Profile* profile);

  
  
  
  static SessionService* GetForProfileForSessionRestore(Profile* profile);

  
  
  
  static void ShutdownForProfile(Profile* profile);

#if defined(UNIT_TEST)
  
  
  static void SetForTestProfile(Profile* profile, SessionService* service) {
    GetInstance()->BrowserContextShutdown(profile);
    GetInstance()->BrowserContextDestroyed(profile);
    GetInstance()->Associate(profile, service);
  }
#endif

  static SessionServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<SessionServiceFactory>;
  FRIEND_TEST_ALL_PREFIXES(SessionCrashedInfoBarDelegateUnitTest,
                           DetachingTabWithCrashedInfoBar);

  SessionServiceFactory();
  virtual ~SessionServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
};

#endif  
