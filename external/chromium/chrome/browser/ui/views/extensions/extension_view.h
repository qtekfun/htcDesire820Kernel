// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_VIEW_H_
#pragma once

#include "build/build_config.h"

#include "third_party/skia/include/core/SkBitmap.h"
#include "views/controls/native/native_view_host.h"

class Browser;
class Extension;
class ExtensionHost;
class ExtensionView;
class RenderViewHost;

class ExtensionView : public views::NativeViewHost {
 public:
  ExtensionView(ExtensionHost* host, Browser* browser);
  ~ExtensionView();

  
  
  class Container {
   public:
    virtual ~Container() {}
    
    virtual void OnExtensionMouseMove(ExtensionView* view) = 0;
    virtual void OnExtensionMouseLeave(ExtensionView* view) = 0;
    virtual void OnExtensionPreferredSizeChanged(ExtensionView* view) {}
  };

  ExtensionHost* host() const { return host_; }
  Browser* browser() const { return browser_; }
  const Extension* extension() const;
  RenderViewHost* render_view_host() const;
  void DidStopLoading();
  void SetIsClipped(bool is_clipped);

  
  void UpdatePreferredSize(const gfx::Size& new_size);
  void HandleMouseMove();
  void HandleMouseLeave();

  
  
  void RenderViewCreated();

  
  void SetBackground(const SkBitmap& background);

  
  void SetContainer(Container* container) { container_ = container; }

  
  virtual void SetVisible(bool is_visible) OVERRIDE;
  virtual void ViewHierarchyChanged(
      bool is_add, views::View *parent, views::View *child) OVERRIDE;

 protected:
  
  virtual void PreferredSizeChanged() OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(const views::KeyEvent& e) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  friend class ExtensionHost;

  
  void CreateWidgetHostView();

  
  void ShowIfCompletelyLoaded();

  
  
  void CleanUp();

  
  
  ExtensionHost* host_;

  
  Browser* browser_;

  
  bool initialized_;

  
  
  SkBitmap pending_background_;

  
  
  gfx::Size pending_preferred_size_;

  
  
  Container* container_;

  
  bool is_clipped_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionView);
};

#endif  
