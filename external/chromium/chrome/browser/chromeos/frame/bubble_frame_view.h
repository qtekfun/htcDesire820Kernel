// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_BUBBLE_FRAME_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_BUBBLE_FRAME_VIEW_H_
#pragma once

#include "chrome/browser/chromeos/frame/bubble_window.h"
#include "views/controls/button/button.h"
#include "views/window/non_client_view.h"

namespace gfx {
class Insets;
class Path;
class Point;
class Rect;
class Size;
}

namespace views {
class ImageButton;
class Label;
class Window;
}

namespace chromeos {

class BubbleFrameView : public views::NonClientFrameView,
                        public views::ButtonListener {
 public:
  BubbleFrameView(views::Window* frame, BubbleWindow::Style style);
  virtual ~BubbleFrameView();

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* window_mask)
      OVERRIDE;
  virtual void EnableClose(bool enable) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;

  
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event)
      OVERRIDE;

  void StartThrobber();
  void StopThrobber();

 private:
  
  views::Window* frame_;

  
  BubbleWindow::Style style_;

  
  views::Label* title_;

  
  gfx::Rect client_view_bounds_;

  
  views::ImageButton* close_button_;

  
  views::Throbber* throbber_;

  DISALLOW_COPY_AND_ASSIGN(BubbleFrameView);
};

}  

#endif  

