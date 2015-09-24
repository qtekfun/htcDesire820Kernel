// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATION_SERVICE_FACTORY_H_
#define CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATION_SERVICE_FACTORY_H_

#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class DesktopNotificationService;
class Profile;

class DesktopNotificationServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static DesktopNotificationService* GetForProfile(Profile* profile);

  static DesktopNotificationServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<DesktopNotificationServiceFactory>;

  DesktopNotificationServiceFactory();
  virtual ~DesktopNotificationServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
};

#endif  
