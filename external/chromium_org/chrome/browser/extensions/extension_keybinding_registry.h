// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_H_

#include <list>
#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_source.h"

class Profile;

namespace ui {
class Accelerator;
}

namespace extensions {

class ActiveTabPermissionGranter;
class Extension;

class ExtensionKeybindingRegistry : public content::NotificationObserver {
 public:
  enum ExtensionFilter {
    ALL_EXTENSIONS,
    PLATFORM_APPS_ONLY
  };

  class Delegate {
   public:
    
    
    virtual ActiveTabPermissionGranter* GetActiveTabPermissionGranter() = 0;
  };

  
  
  ExtensionKeybindingRegistry(Profile* profile,
                              ExtensionFilter extension_filter,
                              Delegate* delegate);

  virtual ~ExtensionKeybindingRegistry();

  
  
  static void SetShortcutHandlingSuspended(bool suspended);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 protected:
  
  
  
  virtual void AddExtensionKeybinding(
      const Extension* extension,
      const std::string& command_name) = 0;
  
  
  void RemoveExtensionKeybinding(
      const Extension* extension,
      const std::string& command_name);
  
  
  virtual void RemoveExtensionKeybindingImpl(
      const ui::Accelerator& accelerator,
      const std::string& command_name) = 0;

  
  void Init();

  
  
  bool ShouldIgnoreCommand(const std::string& command) const;

  
  
  bool NotifyEventTargets(const ui::Accelerator& accelerator);

  
  void CommandExecuted(const std::string& extension_id,
                       const std::string& command);

  
  
  
  
  
  
  
  typedef std::list<std::pair<std::string, std::string> > TargetList;
  typedef std::map<ui::Accelerator, TargetList> EventTargets;
  EventTargets event_targets_;

 private:
  
  bool ExtensionMatchesFilter(const extensions::Extension* extension);

  
  content::NotificationRegistrar registrar_;

  
  Profile* profile_;

  
  ExtensionFilter extension_filter_;

  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionKeybindingRegistry);
};

}  

#endif  
