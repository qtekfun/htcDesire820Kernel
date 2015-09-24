// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_VIEWS_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "ui/base/accelerators/accelerator.h"

class Profile;

namespace extensions {
class Extension;
}

namespace views {
class FocusManager;
}

class ExtensionKeybindingRegistryViews
    : public extensions::ExtensionKeybindingRegistry,
      public ui::AcceleratorTarget {
 public:
  ExtensionKeybindingRegistryViews(Profile* profile,
                                   views::FocusManager* focus_manager,
                                   ExtensionFilter extension_filter,
                                   Delegate* delegate);
  virtual ~ExtensionKeybindingRegistryViews();

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool CanHandleAccelerators() const OVERRIDE;

 private:
  
  virtual void AddExtensionKeybinding(
      const extensions::Extension* extension,
      const std::string& command_name) OVERRIDE;
  virtual void RemoveExtensionKeybindingImpl(
      const ui::Accelerator& accelerator,
      const std::string& command_name) OVERRIDE;

  
  Profile* profile_;

  
  
  views::FocusManager* focus_manager_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionKeybindingRegistryViews);
};

#endif  
