// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_H_

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "build/build_config.h"
#include "chrome/browser/ui/views/frame/browser_non_client_frame_view.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/widget/widget.h"

class AvatarMenuButton;
class BrowserRootView;
class BrowserView;
class NativeBrowserFrame;
class NewAvatarButton;
class NonClientFrameView;
class SystemMenuModelBuilder;

namespace gfx {
class Font;
class Rect;
}

namespace ui {
class MenuModel;
class ThemeProvider;
}

namespace views {
class MenuRunner;
class View;
}

class BrowserFrame
    : public views::Widget,
      public views::ContextMenuController {
 public:
  explicit BrowserFrame(BrowserView* browser_view);
  virtual ~BrowserFrame();

  static const gfx::Font& GetTitleFont();

  
  void InitBrowserFrame();

  
  void SetThemeProvider(scoped_ptr<ui::ThemeProvider> provider);

  
  
  int GetMinimizeButtonOffset() const;

  
  
  gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const;

  
  
  
  
  int GetTopInset() const;

  
  int GetThemeBackgroundXInset() const;

  
  void UpdateThrobber(bool running);

  
  views::View* GetFrameView() const;

  
  virtual views::internal::RootView* CreateRootView() OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView() OVERRIDE;
  virtual bool GetAccelerator(int command_id,
                              ui::Accelerator* accelerator) OVERRIDE;
  virtual ui::ThemeProvider* GetThemeProvider() const OVERRIDE;
  virtual void SchedulePaintInRect(const gfx::Rect& rect) OVERRIDE;
  virtual void OnNativeWidgetActivationChanged(bool active) OVERRIDE;

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& p,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  
  
  bool ShouldLeaveOffsetNearTopBorder();

  AvatarMenuButton* GetAvatarMenuButton();

  NewAvatarButton* GetNewAvatarMenuButton();

  
  
  ui::MenuModel* GetSystemMenuModel();

 private:
  NativeBrowserFrame* native_browser_frame_;

  
  
  
  BrowserRootView* root_view_;

  
  BrowserNonClientFrameView* browser_frame_view_;

  
  BrowserView* browser_view_;

  scoped_ptr<SystemMenuModelBuilder> menu_model_builder_;

  
  
  scoped_ptr<views::MenuRunner> menu_runner_;

  
  
  
  
  scoped_ptr<ui::ThemeProvider> owned_theme_provider_;
  ui::ThemeProvider* theme_provider_;

  DISALLOW_COPY_AND_ASSIGN(BrowserFrame);
};

#endif  
