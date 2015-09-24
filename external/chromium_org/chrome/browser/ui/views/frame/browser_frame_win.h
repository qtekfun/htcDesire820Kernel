// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_WIN_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/minimize_button_metrics_win.h"
#include "chrome/browser/ui/views/frame/native_browser_frame.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/widget/native_widget_win.h"

class BrowserView;
class BrowserWindowPropertyManager;

namespace views {
class NativeMenuWin;
}

class BrowserFrameWin : public views::NativeWidgetWin,
                        public NativeBrowserFrame,
                        public views::ButtonListener {
 public:
  BrowserFrameWin(BrowserFrame* browser_frame, BrowserView* browser_view);
  virtual ~BrowserFrameWin();

  BrowserView* browser_view() const { return browser_view_; }

  
  
  
  static void SetShowState(int state);

 protected:
  
  virtual int GetInitialShowState() const OVERRIDE;
  virtual bool GetClientAreaInsets(gfx::Insets* insets) const OVERRIDE;
  virtual void HandleCreate() OVERRIDE;
  virtual void HandleFrameChanged() OVERRIDE;
  virtual bool PreHandleMSG(UINT message,
                            WPARAM w_param,
                            LPARAM l_param,
                            LRESULT* result) OVERRIDE;
  virtual void PostHandleMSG(UINT message,
                             WPARAM w_param,
                             LPARAM l_param) OVERRIDE;
  virtual bool ShouldUseNativeFrame() const OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void ShowMaximizedWithBounds(
      const gfx::Rect& restored_bounds) OVERRIDE;
  virtual void ShowWithWindowState(ui::WindowShowState show_state) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void FrameTypeChanged() OVERRIDE;
  virtual void SetFullscreen(bool fullscreen) OVERRIDE;
  virtual void Activate() OVERRIDE;

  
  virtual views::NativeWidget* AsNativeWidget() OVERRIDE;
  virtual const views::NativeWidget* AsNativeWidget() const OVERRIDE;
  virtual bool UsesNativeSystemMenu() const OVERRIDE;
  virtual int GetMinimizeButtonOffset() const OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

 private:
  
  void UpdateDWMFrame();

  
  void HandleMetroNavSearchRequest(WPARAM w_param, LPARAM l_param);

  
  void GetMetroCurrentTabInfo(WPARAM w_param);

  
  
  void AdjustFrameForImmersiveMode();

  
  void CloseImmersiveFrame();

  views::NativeMenuWin* GetSystemMenu();

  
  BrowserView* browser_view_;

  BrowserFrame* browser_frame_;

  
  scoped_ptr<views::NativeMenuWin> system_menu_;

  MinimizeButtonMetrics minimize_button_metrics_;

  scoped_ptr<BrowserWindowPropertyManager> browser_window_property_manager_;

  DISALLOW_COPY_AND_ASSIGN(BrowserFrameWin);
};

views::Button* MakeWindowSwitcherButton(views::ButtonListener* listener,
                                        bool is_off_the_record);

#endif  
