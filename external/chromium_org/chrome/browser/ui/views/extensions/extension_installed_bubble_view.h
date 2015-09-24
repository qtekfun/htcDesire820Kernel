// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_VIEW_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/extensions/extension_installed_bubble.h"
#include "ui/views/bubble/bubble_delegate.h"

class Browser;

namespace extensions {
class Extension;
}

class ExtensionInstalledBubbleView
    : public ExtensionInstalledBubble::Delegate,
      public views::BubbleDelegateView {
 public:
  
  
  
  
  static void Show(const extensions::Extension* extension,
                   Browser* browser,
                   const SkBitmap& icon);

 private:
  ExtensionInstalledBubbleView(const extensions::Extension* extension,
                               Browser* browser,
                               const SkBitmap& icon);

  virtual ~ExtensionInstalledBubbleView();

  
  virtual bool MaybeShowNow() OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

  
  virtual gfx::Rect GetAnchorRect() OVERRIDE;

  ExtensionInstalledBubble bubble_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstalledBubbleView);
};

#endif  
