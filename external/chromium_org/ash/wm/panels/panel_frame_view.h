// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_PANELS_PANEL_FRAME_VIEW_H_
#define ASH_WM_PANELS_PANEL_FRAME_VIEW_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "ui/gfx/font.h"
#include "ui/views/window/non_client_view.h"

namespace views {
class ImageView;
}

namespace ash {

class FrameCaptionButtonContainerView;
class FrameBorderHitTestController;
class HeaderPainter;

class ASH_EXPORT PanelFrameView : public views::NonClientFrameView {
 public:
  
  static const char kViewClassName[];

  enum FrameType {
    FRAME_NONE,
    FRAME_ASH
  };

  PanelFrameView(views::Widget* frame, FrameType frame_type);
  virtual ~PanelFrameView();

  
  virtual const char* GetClassName() const OVERRIDE;

 private:
  void InitHeaderPainter();

  
  int NonClientTopBorderHeight() const;

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size,
                             gfx::Path* window_mask) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;

  
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  
  views::Widget* frame_;
  FrameCaptionButtonContainerView* caption_button_container_;
  views::ImageView* window_icon_;
  gfx::Rect client_view_bounds_;
  const gfx::Font title_font_;

  
  scoped_ptr<HeaderPainter> header_painter_;

  
  scoped_ptr<FrameBorderHitTestController> frame_border_hit_test_controller_;

  DISALLOW_COPY_AND_ASSIGN(PanelFrameView);
};

}

#endif 
