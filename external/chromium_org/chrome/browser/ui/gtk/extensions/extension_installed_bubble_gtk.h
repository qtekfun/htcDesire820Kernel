// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_GTK_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/extensions/extension_installed_bubble.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "chrome/browser/ui/gtk/custom_button.h"

class Browser;
class SkBitmap;

namespace extensions {
class Extension;
}

class ExtensionInstalledBubbleGtk
    : public ExtensionInstalledBubble::Delegate,
      public BubbleDelegateGtk {
 public:
  virtual ~ExtensionInstalledBubbleGtk();

  
  
  
  
  static void Show(const extensions::Extension* extension,
                   Browser* browser,
                   const SkBitmap& icon);

 private:
  ExtensionInstalledBubbleGtk(const extensions::Extension* extension,
                              Browser* browser,
                              const SkBitmap& icon);

  
  CHROMEGTK_CALLBACK_0(ExtensionInstalledBubbleGtk, void, OnDestroy);

  
  virtual bool MaybeShowNow() OVERRIDE;

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

  static void OnButtonClick(GtkWidget* button,
                            ExtensionInstalledBubbleGtk* toolbar);

  
  CHROMEGTK_CALLBACK_0(ExtensionInstalledBubbleGtk, void, OnLinkClicked);

  
  scoped_ptr<CustomDrawButton> close_button_;

  ExtensionInstalledBubble bubble_;
  BubbleGtk* gtk_bubble_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstalledBubbleGtk);
};

#endif  
