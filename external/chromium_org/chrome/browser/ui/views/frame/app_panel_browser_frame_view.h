// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_APP_PANEL_BROWSER_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_APP_PANEL_BROWSER_FRAME_VIEW_H_

#include "chrome/browser/ui/views/frame/browser_non_client_frame_view.h"
#include "chrome/browser/ui/views/tab_icon_view_model.h"
#include "ui/views/controls/button/button.h"

class BrowserFrame;
class BrowserView;
class TabIconView;

namespace views {
class ImageButton;
}

class AppPanelBrowserFrameView : public BrowserNonClientFrameView,
                                 public views::ButtonListener,
                                 public chrome::TabIconViewModel {
 public:
  
  AppPanelBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  virtual ~AppPanelBrowserFrameView();

  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const OVERRIDE;
  virtual int GetTopInset() const OVERRIDE;
  virtual int GetThemeBackgroundXInset() const OVERRIDE;
  virtual void UpdateThrobber(bool running) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

 protected:
  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size,
                             gfx::Path* window_mask) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool ShouldTabIconViewAnimate() const OVERRIDE;
  virtual gfx::ImageSkia GetFaviconForTabIconView() OVERRIDE;

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

  
  gfx::Rect client_view_bounds_;

  DISALLOW_COPY_AND_ASSIGN(AppPanelBrowserFrameView);
};

#endif  
