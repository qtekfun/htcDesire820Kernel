// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_VIEW_GTK_H_
#pragma once

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

class Browser;
class ExtensionHost;
class RenderViewHost;
class RenderWidgetHostViewGtk;
class SkBitmap;

class ExtensionViewGtk {
 public:
  ExtensionViewGtk(ExtensionHost* extension_host, Browser* browser);

  class Container {
   public:
    virtual ~Container() {}
    virtual void OnExtensionPreferredSizeChanged(ExtensionViewGtk* view,
                                                 const gfx::Size& new_size) {}
  };

  void Init();

  gfx::NativeView native_view();
  Browser* browser() const { return browser_; }

  void SetBackground(const SkBitmap& background);

  
  void SetContainer(Container* container) { container_ = container; }

  
  
  void UpdatePreferredSize(const gfx::Size& new_size);

  
  
  void RenderViewCreated();

  RenderViewHost* render_view_host() const;

 private:
  void CreateWidgetHostView();

  Browser* browser_;

  ExtensionHost* extension_host_;

  RenderWidgetHostViewGtk* render_widget_host_view_;

  
  
  SkBitmap pending_background_;

  
  Container* container_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionViewGtk);
};

#endif  
