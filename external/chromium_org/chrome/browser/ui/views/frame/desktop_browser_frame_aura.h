// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_DESKTOP_BROWSER_FRAME_AURA_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_DESKTOP_BROWSER_FRAME_AURA_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/frame/native_browser_frame.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

class BrowserDesktopRootWindowHost;
class BrowserFrame;
class BrowserView;

namespace aura {
namespace client {
class UserActionClient;
}
}

namespace views {
namespace corewm {
class VisibilityController;
}
}

class DesktopBrowserFrameAura : public views::DesktopNativeWidgetAura,
                                public NativeBrowserFrame {
 public:
  DesktopBrowserFrameAura(BrowserFrame* browser_frame,
                          BrowserView* browser_view);

  BrowserView* browser_view() const { return browser_view_; }

 protected:
  virtual ~DesktopBrowserFrameAura();

  
  virtual void OnHostClosed() OVERRIDE;
  virtual void InitNativeWidget(
      const views::Widget::InitParams& params) OVERRIDE;

  
  virtual views::NativeWidget* AsNativeWidget() OVERRIDE;
  virtual const views::NativeWidget* AsNativeWidget() const OVERRIDE;
  virtual bool UsesNativeSystemMenu() const OVERRIDE;
  virtual int GetMinimizeButtonOffset() const OVERRIDE;

 private:
  
  BrowserView* browser_view_;
  BrowserFrame* browser_frame_;

  
  BrowserDesktopRootWindowHost* browser_desktop_root_window_host_;

  scoped_ptr<aura::client::UserActionClient> user_action_client_;
  scoped_ptr<views::corewm::VisibilityController> visibility_controller_;

  DISALLOW_COPY_AND_ASSIGN(DesktopBrowserFrameAura);
};

#endif  
