// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_VIEW_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_VIEW_VIEWS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/native_web_keyboard_event.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/webview/unhandled_keyboard_event_handler.h"

class Browser;

namespace content {
class RenderViewHost;
}

namespace extensions {
class Extension;
class ExtensionHost;
}

class ExtensionViewViews : public views::NativeViewHost {
 public:
  ExtensionViewViews(extensions::ExtensionHost* host, Browser* browser);
  virtual ~ExtensionViewViews();

  
  
  class Container {
   public:
    virtual ~Container() {}
    virtual void OnExtensionSizeChanged(ExtensionViewViews* view) {}
    virtual void OnViewWasResized() {}
  };

  extensions::ExtensionHost* host() const { return host_; }
  Browser* browser() const { return browser_; }
  const extensions::Extension* extension() const;
  content::RenderViewHost* render_view_host() const;
  void DidStopLoading();
  void SetIsClipped(bool is_clipped);

  
  void SetMinimumSize(const gfx::Size& min_size);

  
  void ResizeDueToAutoResize(const gfx::Size& new_size);

  
  
  void RenderViewCreated();

  
  void SetContainer(Container* container) { container_ = container; }

  
  void HandleKeyboardEvent(const content::NativeWebKeyboardEvent& event);

  
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;
  virtual void SetVisible(bool is_visible) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 private:
  
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void PreferredSizeChanged() OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

 private:
  friend class extensions::ExtensionHost;

  
  void CreateWidgetHostView();

  
  void ShowIfCompletelyLoaded();

  
  
  void CleanUp();

  
  
  extensions::ExtensionHost* host_;

  
  Browser* browser_;

  
  bool initialized_;

  
  
  gfx::Size pending_preferred_size_;
  gfx::Size minimum_size_;

  
  
  Container* container_;

  
  bool is_clipped_;

  
  
  views::UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionViewViews);
};

#endif  
