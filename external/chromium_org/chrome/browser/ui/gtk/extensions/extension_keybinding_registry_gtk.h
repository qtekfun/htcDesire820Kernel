// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_KEYBINDING_REGISTRY_GTK_H_

#include <gdk/gdk.h>
#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/native_widget_types.h"

class Profile;

namespace extensions {
class Extension;
}

typedef struct _GtkAccelGroup GtkAccelGroup;
typedef struct _GdkEventKey GdkEventKey;


class ExtensionKeybindingRegistryGtk
    : public extensions::ExtensionKeybindingRegistry {
 public:
  ExtensionKeybindingRegistryGtk(Profile* profile,
                                 gfx::NativeWindow window,
                                 ExtensionFilter extension_filter,
                                 Delegate* delegate);
  virtual ~ExtensionKeybindingRegistryGtk();

  static void set_shortcut_handling_suspended(bool suspended) {
    shortcut_handling_suspended_ = suspended;
  }
  static bool shortcut_handling_suspended() {
    return shortcut_handling_suspended_;
  }

  
  
  gboolean HasPriorityHandler(const GdkEventKey* event) const;

 protected:
  
  virtual void AddExtensionKeybinding(
      const extensions::Extension* extension,
      const std::string& command_name) OVERRIDE;
  virtual void RemoveExtensionKeybindingImpl(
      const ui::Accelerator& accelerator,
      const std::string& command_name) OVERRIDE;

 private:
  
  
  CHROMEG_CALLBACK_3(ExtensionKeybindingRegistryGtk, gboolean, OnGtkAccelerator,
                     GtkAccelGroup*, GObject*, guint, GdkModifierType);

  
  
  
  
  
  static bool shortcut_handling_suspended_;

  
  Profile* profile_;

  
  gfx::NativeWindow window_;

  
  GtkAccelGroup* accel_group_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionKeybindingRegistryGtk);
};

#endif  
