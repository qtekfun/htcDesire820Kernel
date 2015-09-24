// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_
#pragma once

#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/browser_non_client_frame_view.h"
#include "chrome/browser/ui/views/tab_icon_view.h"
#include "views/controls/button/button.h"
#include "views/window/non_client_view.h"

class BrowserView;
namespace gfx {
class Font;
}
class TabContents;
namespace views {
class ImageButton;
class ImageView;
}

class OpaqueBrowserFrameView : public BrowserNonClientFrameView,
                               public views::ButtonListener,
                               public TabIconView::TabIconViewModel {
 public:
  
  OpaqueBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  virtual ~OpaqueBrowserFrameView();

  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const OVERRIDE;
  virtual int GetHorizontalTabStripVerticalOffset(bool restored) const OVERRIDE;
  virtual void UpdateThrobber(bool running) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

 protected:
  BrowserView* browser_view() const { return browser_view_; }
  views::ImageButton* minimize_button() const { return minimize_button_; }
  views::ImageButton* maximize_button() const { return maximize_button_; }
  views::ImageButton* restore_button() const { return restore_button_; }
  views::ImageButton* close_button() const { return close_button_; }

  
  
  virtual int GetReservedHeight() const;
  virtual gfx::Rect GetBoundsForReservedArea() const;

  
  
  
  
  
  int NonClientTopBorderHeight(bool restored, bool ignore_vertical_tabs) const;

  
  
  
  
  
  virtual void ModifyMaximizedFramePainting(
      int* theme_offset,
      SkBitmap** left_corner,
      SkBitmap** right_corner);

  
  BrowserFrame* frame() { return frame_; }
  BrowserView* browser_view() { return browser_view_; }

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual bool AlwaysUseNativeFrame() const OVERRIDE;
  virtual bool AlwaysUseCustomFrame() const OVERRIDE;
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
  virtual bool HitTest(const gfx::Point& l) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event)
      OVERRIDE;

  
  virtual bool ShouldTabIconViewAnimate() const OVERRIDE;
  virtual SkBitmap GetFaviconForTabIconView() OVERRIDE;

 private:
  
  
  
  int FrameBorderThickness(bool restored) const;

  
  
  int TopResizeHeight() const;

  
  
  int NonClientBorderThickness() const;

  
  
  int CaptionButtonY(bool restored) const;

  
  
  
  int TitlebarBottomThickness(bool restored) const;

  
  
  int IconSize() const;

  
  
  gfx::Rect IconBounds() const;

  
  
  
  void PaintRestoredFrameBorder(gfx::Canvas* canvas);
  void PaintMaximizedFrameBorder(gfx::Canvas* canvas);
  void PaintTitleBar(gfx::Canvas* canvas);
  void PaintToolbarBackground(gfx::Canvas* canvas);
  void PaintOTRAvatar(gfx::Canvas* canvas);
  void PaintRestoredClientEdge(gfx::Canvas* canvas);

  
  void LayoutWindowControls();
  void LayoutTitleBar();
  void LayoutOTRAvatar();

  
  gfx::Rect CalculateClientAreaBounds(int width, int height) const;

  
  gfx::Rect title_bounds_;

  
  gfx::Rect otr_avatar_bounds_;

  
  views::ImageButton* minimize_button_;
  views::ImageButton* maximize_button_;
  views::ImageButton* restore_button_;
  views::ImageButton* close_button_;

  
  TabIconView* window_icon_;

  
  BrowserFrame* frame_;

  
  BrowserView* browser_view_;

  
  gfx::Rect client_view_bounds_;

  DISALLOW_COPY_AND_ASSIGN(OpaqueBrowserFrameView);
};

#endif  
