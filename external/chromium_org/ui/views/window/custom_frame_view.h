// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_CUSTOM_FRAME_VIEW_H_
#define UI_VIEWS_WINDOW_CUSTOM_FRAME_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/non_client_view.h"

namespace gfx {
class ImageSkia;
}

namespace views {

class FrameBackground;
class ImageButton;
class Widget;

class CustomFrameView : public NonClientFrameView,
                        public ButtonListener {
 public:
  CustomFrameView();
  virtual ~CustomFrameView();

  void Init(Widget* frame);

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size,
                             gfx::Path* window_mask) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

 private:
  
  
  int FrameBorderThickness() const;

  
  
  int NonClientBorderThickness() const;

  
  
  int NonClientTopBorderHeight() const;

  
  int CaptionButtonY() const;

  
  
  int TitlebarBottomThickness() const;

  
  
  int IconSize() const;

  
  
  gfx::Rect IconBounds() const;

  
  
  bool ShouldShowClientEdge() const;

  
  void PaintRestoredFrameBorder(gfx::Canvas* canvas);
  void PaintMaximizedFrameBorder(gfx::Canvas* canvas);
  void PaintTitleBar(gfx::Canvas* canvas);
  void PaintRestoredClientEdge(gfx::Canvas* canvas);

  
  SkColor GetFrameColor() const;
  const gfx::ImageSkia* GetFrameImage() const;

  
  void LayoutWindowControls();
  void LayoutTitleBar();
  void LayoutClientView();

  
  
  ImageButton* InitWindowCaptionButton(int accessibility_string_id,
                                       int normal_image_id,
                                       int hot_image_id,
                                       int pushed_image_id);

  
  gfx::Rect client_view_bounds_;

  
  gfx::Rect title_bounds_;

  
  Widget* frame_;

  
  ImageButton* window_icon_;

  
  ImageButton* minimize_button_;
  ImageButton* maximize_button_;
  ImageButton* restore_button_;
  ImageButton* close_button_;

  
  bool should_show_maximize_button_;

  
  scoped_ptr<FrameBackground> frame_background_;

  DISALLOW_COPY_AND_ASSIGN(CustomFrameView);
};

}  

#endif  
