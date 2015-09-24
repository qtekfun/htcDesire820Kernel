// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_APP_PANEL_BROWSER_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_APP_PANEL_BROWSER_FRAME_VIEW_H_
#pragma once

#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/browser_non_client_frame_view.h"
#include "chrome/browser/ui/views/tab_icon_view.h"
#include "views/controls/button/button.h"
#include "views/window/non_client_view.h"

class BrowserView;
class TabContents;
namespace gfx {
class Font;
}
namespace views {
class ImageButton;
class ImageView;
}
class AppPanelBrowserFrameView : public BrowserNonClientFrameView,
                                 public views::ButtonListener,
                                 public TabIconView::TabIconViewModel {
 public:
  
  AppPanelBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  virtual ~AppPanelBrowserFrameView();

  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const OVERRIDE;
  virtual int GetHorizontalTabStripVerticalOffset(bool restored) const OVERRIDE;
  virtual void UpdateThrobber(bool running) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

 protected:
  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual bool AlwaysUseCustomFrame() const OVERRIDE;
  virtual bool AlwaysUseNativeFrame() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* window_mask)
      OVERRIDE;
  virtual void EnableClose(bool enable) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event)
      OVERRIDE;

  
  virtual bool ShouldTabIconViewAnimate() const OVERRIDE;
  virtual SkBitmap GetFaviconForTabIconView() OVERRIDE;

 private:
  
  
  int FrameBorderThickness() const;

  
  
  int NonClientBorderThickness() const;

  
  
  int NonClientTopBorderHeight() const;

  
  
  int TitlebarBottomThickness() const;

  
  int IconSize() const;

  
  gfx::Rect IconBounds() const;

  
  
  
  void PaintRestoredFrameBorder(gfx::Canvas* canvas);
  void PaintMaximizedFrameBorder(gfx::Canvas* canvas);
  void PaintTitleBar(gfx::Canvas* canvas);
  void PaintRestoredClientEdge(gfx::Canvas* canvas);

  
  void LayoutWindowControls();
  void LayoutTitleBar();

  
  gfx::Rect CalculateClientAreaBounds(int width, int height) const;

  
  gfx::Rect title_bounds_;

  
  views::ImageButton* close_button_;

  
  TabIconView* window_icon_;

  
  BrowserFrame* frame_;

  
  BrowserView* browser_view_;

  
  gfx::Rect client_view_bounds_;

  
  std::wstring accessible_name_;

  static void InitAppWindowResources();
  static gfx::Font* title_font_;

  DISALLOW_COPY_AND_ASSIGN(AppPanelBrowserFrameView);
};

#endif  
