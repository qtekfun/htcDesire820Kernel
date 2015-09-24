// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_COCOA_H_

#include <string>
#include <utility>

#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/gfx/native_widget_types.h"

class Profile;

namespace content {
  struct NativeWebKeyboardEvent;
}
namespace extensions {
  class Extension;
}


class ExtensionKeybindingRegistryCocoa
    : public extensions::ExtensionKeybindingRegistry {
 public:
  ExtensionKeybindingRegistryCocoa(Profile* profile,
                                   gfx::NativeWindow window,
                                   ExtensionFilter extension_filter,
                                   Delegate* delegate);
  virtual ~ExtensionKeybindingRegistryCocoa();

  static void set_shortcut_handling_suspended(bool suspended) {
    shortcut_handling_suspended_ = suspended;
  }
  static bool shortcut_handling_suspended() {
    return shortcut_handling_suspended_;
  }

  
  
  
  bool ProcessKeyEvent(const content::NativeWebKeyboardEvent& event);

 protected:
  
  virtual void AddExtensionKeybinding(
      const extensions::Extension* extension,
      const std::string& command_name) OVERRIDE;
  virtual void RemoveExtensionKeybindingImpl(
      const ui::Accelerator& accelerator,
      const std::string& command_name) OVERRIDE;

 private:
  
  
  
  
  
  static bool shortcut_handling_suspended_;

  
  Profile* profile_;

  
  gfx::NativeWindow window_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionKeybindingRegistryCocoa);
};

#endif  
