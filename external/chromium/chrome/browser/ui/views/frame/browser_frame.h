// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_H_
#pragma once

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "build/build_config.h"
#include "chrome/browser/ui/views/frame/native_browser_frame_delegate.h"

class AeroGlassNonClientView;
class BrowserNonClientFrameView;
class BrowserRootView;
class BrowserView;
class NativeBrowserFrame;
class NonClientFrameView;
class Profile;

namespace gfx {
class Font;
class Rect;
}

namespace ui {
class ThemeProvider;
}

namespace views {
class View;
class Window;
}

class BrowserFrame : public NativeBrowserFrameDelegate {
 public:
  virtual ~BrowserFrame();

  
  
  static BrowserFrame* Create(BrowserView* browser_view, Profile* profile);

  static const gfx::Font& GetTitleFont();

  
  
  views::Window* GetWindow();

  
  
  int GetMinimizeButtonOffset() const;

  
  
  gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const;

  
  
  
  int GetHorizontalTabStripVerticalOffset(bool restored) const;

  
  void UpdateThrobber(bool running);

  
  ui::ThemeProvider* GetThemeProviderForFrame() const;

  
  
  
  bool AlwaysUseNativeFrame() const;

  
  views::View* GetFrameView() const;

  
  
  void TabStripDisplayModeChanged();

 protected:
  
  virtual views::RootView* DelegateCreateRootView() OVERRIDE;
  virtual views::NonClientFrameView* DelegateCreateFrameViewForWindow()
      OVERRIDE;

  
  
  void set_native_browser_frame(NativeBrowserFrame* native_browser_frame) {
    native_browser_frame_ = native_browser_frame;
  }

  explicit BrowserFrame(BrowserView* browser_view);

 private:
  NativeBrowserFrame* native_browser_frame_;

  
  
  
  BrowserRootView* root_view_;

  
  BrowserNonClientFrameView* browser_frame_view_;

  
  BrowserView* browser_view_;

  DISALLOW_COPY_AND_ASSIGN(BrowserFrame);
};

#endif  
