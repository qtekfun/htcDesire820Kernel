// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_PARTIAL_SCREENSHOT_VIEW_H_
#define ASH_WM_PARTIAL_SCREENSHOT_VIEW_H_

#include <vector>

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/widget_delegate.h"

namespace ash {
class ScreenshotDelegate;

class ASH_EXPORT PartialScreenshotView : public views::WidgetDelegateView {
 public:
  
  
  
  static std::vector<PartialScreenshotView*>
      StartPartialScreenshot(ScreenshotDelegate* screenshot_delegate);

 private:
  FRIEND_TEST_ALL_PREFIXES(PartialScreenshotViewTest, BasicMouse);
  FRIEND_TEST_ALL_PREFIXES(PartialScreenshotViewTest, BasicTouch);

  class OverlayDelegate;

  PartialScreenshotView(OverlayDelegate* overlay_delegate,
                        ScreenshotDelegate* screenshot_delegate);
  virtual ~PartialScreenshotView();

  
  void Init(aura::Window* root_window);

  
  gfx::Rect GetScreenshotRect() const;

  void OnSelectionStarted(const gfx::Point& position);
  void OnSelectionChanged(const gfx::Point& position);
  void OnSelectionFinished();

  
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  bool is_dragging_;
  gfx::Point start_position_;
  gfx::Point current_position_;

  
  OverlayDelegate* overlay_delegate_;

  
  ScreenshotDelegate* screenshot_delegate_;

  DISALLOW_COPY_AND_ASSIGN(PartialScreenshotView);
};

}  

#endif  
