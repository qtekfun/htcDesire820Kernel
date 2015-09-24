// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INDICATOR_SYSTEM_INDICATOR_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INDICATOR_SYSTEM_INDICATOR_MANAGER_H_

#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/extensions/extension_action_icon_factory.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/event_router.h"

class ExtensionAction;
class Profile;
class StatusTray;

FORWARD_DECLARE_TEST(SystemIndicatorApiTest, SystemIndicator);

namespace extensions {

class ExtensionIndicatorIcon;

class SystemIndicatorManager : public content::NotificationObserver,
                               public BrowserContextKeyedService {
 public:
  SystemIndicatorManager(Profile* profile, StatusTray* status_tray);
  virtual ~SystemIndicatorManager();

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(::SystemIndicatorApiTest, SystemIndicator);

  
  
  void OnSystemIndicatorChanged(const ExtensionAction* extension_action);

  
  
  bool SendClickEventToExtensionForTest(const std::string extension_id);

  
  
  void CreateOrUpdateIndicator(
      const Extension* extension,
      const ExtensionAction* extension_action);

  
  void RemoveIndicator(const std::string &extension_id);

  typedef std::map<const std::string, linked_ptr<ExtensionIndicatorIcon> >
      SystemIndicatorMap;

  Profile* profile_;
  StatusTray* status_tray_;
  SystemIndicatorMap system_indicators_;
  content::NotificationRegistrar registrar_;
  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(SystemIndicatorManager);
};

}  

#endif  
