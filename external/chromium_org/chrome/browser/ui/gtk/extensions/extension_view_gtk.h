// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_VIEW_GTK_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

class Browser;
class SkBitmap;

namespace content {
class RenderViewHost;
}

namespace extensions {
class ExtensionHost;
}

class ExtensionViewGtk {
 public:
  ExtensionViewGtk(extensions::ExtensionHost* extension_host, Browser* browser);

  class Container {
   public:
    virtual ~Container() {}
    virtual void OnExtensionSizeChanged(ExtensionViewGtk* view,
                                        const gfx::Size& new_size) {}
  };

  void Init();

  gfx::NativeView native_view();
  Browser* browser() const { return browser_; }

  void SetBackground(const SkBitmap& background);

  
  void SetContainer(Container* container) { container_ = container; }

  
  
  void ResizeDueToAutoResize(const gfx::Size& new_size);

  
  
  void RenderViewCreated();

  content::RenderViewHost* render_view_host() const;

 private:
  void CreateWidgetHostView();

  Browser* browser_;

  extensions::ExtensionHost* extension_host_;

  
  
  SkBitmap pending_background_;

  
  Container* container_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionViewGtk);
};

#endif  
