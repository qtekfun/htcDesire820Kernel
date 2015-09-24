// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_COMMANDS_GLOBAL_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_COMMANDS_GLOBAL_REGISTRY_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "chrome/browser/extensions/global_shortcut_listener.h"
#include "ui/base/accelerators/accelerator.h"

class Profile;

namespace extensions {
class Extension;
}

namespace extensions {

class ExtensionCommandsGlobalRegistry
    : public ProfileKeyedAPI,
      public ExtensionKeybindingRegistry,
      public GlobalShortcutListener::Observer {
 public:
  
  static ProfileKeyedAPIFactory<
      ExtensionCommandsGlobalRegistry>* GetFactoryInstance();

  
  
  static ExtensionCommandsGlobalRegistry* Get(Profile* profile);

  explicit ExtensionCommandsGlobalRegistry(Profile* profile);
  virtual ~ExtensionCommandsGlobalRegistry();

 private:
  friend class ProfileKeyedAPIFactory<ExtensionCommandsGlobalRegistry>;

  
  static const char* service_name() {
    return "ExtensionCommandsGlobalRegistry";
  }

  
  virtual void AddExtensionKeybinding(
      const Extension* extension,
      const std::string& command_name) OVERRIDE;
  virtual void RemoveExtensionKeybindingImpl(
      const ui::Accelerator& accelerator,
      const std::string& command_name) OVERRIDE;

  
  
  virtual void OnKeyPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionCommandsGlobalRegistry);
};

}  

#endif  
