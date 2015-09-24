// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_WIN_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/native_browser_frame.h"
#include "views/window/window_win.h"

class BrowserView;
class Profile;

class BrowserFrameWin : public BrowserFrame,
                        public views::WindowWin,
                        public NativeBrowserFrame {
 public:
  
  
  BrowserFrameWin(BrowserView* browser_view, Profile* profile);
  virtual ~BrowserFrameWin();

  
  
  void InitBrowserFrame();

  BrowserView* browser_view() const { return browser_view_; }

  
  
  
  static void SetShowState(int state);

 protected:
  
  virtual int GetShowState() const OVERRIDE;
  virtual gfx::Insets GetClientAreaInsets() const OVERRIDE;
  virtual bool GetAccelerator(int cmd_id,
                              ui::Accelerator* accelerator) OVERRIDE;
  virtual void OnEndSession(BOOL ending, UINT logoff) OVERRIDE;
  virtual void OnInitMenuPopup(HMENU menu,
                               UINT position,
                               BOOL is_system_menu) OVERRIDE;
  virtual void OnWindowPosChanged(WINDOWPOS* window_pos) OVERRIDE;
  virtual ui::ThemeProvider* GetThemeProvider() const OVERRIDE;
  virtual void OnScreenReaderDetected() OVERRIDE;

  
  virtual void Activate() OVERRIDE;
  virtual bool IsAppWindow() const OVERRIDE { return true; }
  virtual void UpdateFrameAfterFrameChange() OVERRIDE;
  virtual views::RootView* CreateRootView() OVERRIDE;
  virtual views::NonClientFrameView* CreateFrameViewForWindow() OVERRIDE;
  virtual bool ShouldUseNativeFrame() const OVERRIDE;

  
  virtual views::NativeWindow* AsNativeWindow() OVERRIDE;
  virtual const views::NativeWindow* AsNativeWindow() const OVERRIDE;
  virtual BrowserNonClientFrameView* CreateBrowserNonClientFrameView() OVERRIDE;
  virtual int GetMinimizeButtonOffset() const OVERRIDE;
  virtual ui::ThemeProvider* GetThemeProviderForFrame() const OVERRIDE;
  virtual bool AlwaysUseNativeFrame() const OVERRIDE;
  virtual void TabStripDisplayModeChanged() OVERRIDE;

 private:
  
  void UpdateDWMFrame();

  NativeBrowserFrameDelegate* delegate_;

  
  BrowserView* browser_view_;

  DISALLOW_COPY_AND_ASSIGN(BrowserFrameWin);
};

#endif  
