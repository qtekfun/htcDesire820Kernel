// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_VIEW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_VIEW_MAC_H_

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

class ExtensionViewMac {
 public:
  class Container {
   public:
    virtual ~Container() {}
    virtual void OnExtensionSizeChanged(ExtensionViewMac* view,
                                        const gfx::Size& new_size) {}
    virtual void OnExtensionViewDidShow(ExtensionViewMac* view) {};
  };

  ExtensionViewMac(extensions::ExtensionHost* extension_host, Browser* browser);
  ~ExtensionViewMac();

  
  
  void Init();

  
  gfx::NativeView native_view();

  
  Browser* browser() const { return browser_; }

  
  
  void DidStopLoading();

  
  void set_container(Container* container) { container_ = container; }

  
  
  void ResizeDueToAutoResize(const gfx::Size& new_size);

  
  
  void RenderViewCreated();

  
  void WindowFrameChanged();

  
  
  
  
  static const CGFloat kMinWidth;
  static const CGFloat kMinHeight;
  static const CGFloat kMaxWidth;
  static const CGFloat kMaxHeight;

 private:
  content::RenderViewHost* render_view_host() const;

  void CreateWidgetHostView();

  
  void ShowIfCompletelyLoaded();

  Browser* browser_;  

  extensions::ExtensionHost* extension_host_;  

  
  
  gfx::Size pending_preferred_size_;

  Container* container_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionViewMac);
};

#endif  
