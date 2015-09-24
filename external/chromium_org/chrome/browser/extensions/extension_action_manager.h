// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ACTION_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ACTION_MANAGER_H_

#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class ExtensionAction;
class Profile;

namespace extensions {

class Extension;

class ExtensionActionManager : public BrowserContextKeyedService,
                               public content::NotificationObserver {
 public:
  explicit ExtensionActionManager(Profile* profile);
  virtual ~ExtensionActionManager();

  
  
  static ExtensionActionManager* Get(Profile* profile);

  
  
  
  ExtensionAction* GetPageAction(const extensions::Extension& extension) const;
  ExtensionAction* GetBrowserAction(
      const extensions::Extension& extension) const;
  ExtensionAction* GetScriptBadge(const extensions::Extension& extension) const;
  ExtensionAction* GetSystemIndicator(
      const extensions::Extension& extension) const;

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;
  Profile* profile_;

  
  
  
  
  typedef std::map<std::string, linked_ptr<ExtensionAction> > ExtIdToActionMap;
  mutable ExtIdToActionMap page_actions_;
  mutable ExtIdToActionMap browser_actions_;
  mutable ExtIdToActionMap script_badges_;
  mutable ExtIdToActionMap system_indicators_;
};

}  

#endif  
