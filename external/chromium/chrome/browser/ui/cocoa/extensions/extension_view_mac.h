// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_VIEW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_VIEW_MAC_H_
#pragma once

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

class Browser;
class ExtensionHost;
class RenderViewHost;
class RenderWidgetHostViewMac;
class SkBitmap;

class ExtensionViewMac {
 public:
  ExtensionViewMac(ExtensionHost* extension_host, Browser* browser);
  ~ExtensionViewMac();

  
  
  void Init();

  
  gfx::NativeView native_view();

  
  Browser* browser() const { return browser_; }

  
  bool is_toolstrip() const { return is_toolstrip_; }
  void set_is_toolstrip(bool is_toolstrip) { is_toolstrip_ = is_toolstrip; }

  
  void SetBackground(const SkBitmap& background);

  
  
  void UpdatePreferredSize(const gfx::Size& new_size);

  
  
  void RenderViewCreated();

  
  void WindowFrameChanged();

  
  
  
  
  static const CGFloat kMinWidth;
  static const CGFloat kMinHeight;
  static const CGFloat kMaxWidth;
  static const CGFloat kMaxHeight;

 private:
  RenderViewHost* render_view_host() const;

  void CreateWidgetHostView();

  
  bool is_toolstrip_;

  Browser* browser_;  

  ExtensionHost* extension_host_;  

  
  
  RenderWidgetHostViewMac* render_widget_host_view_;

  
  
  SkBitmap pending_background_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionViewMac);
};

#endif  
