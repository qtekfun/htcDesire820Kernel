// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_MOUSE_CURSOR_EVENT_FILTER_H
#define ASH_DISPLAY_MOUSE_CURSOR_EVENT_FILTER_H

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/rect.h"

namespace aura {
class RootWindow;
class Window;
}

namespace ash {
class DisplayController;

namespace internal {
class SharedDisplayEdgeIndicator;

class ASH_EXPORT MouseCursorEventFilter : public ui::EventHandler {
 public:
  enum MouseWarpMode {
    WARP_ALWAYS,   
    WARP_DRAG,     
                   
                   
    WARP_NONE,     
  };

  MouseCursorEventFilter();
  virtual ~MouseCursorEventFilter();

  void set_mouse_warp_mode(MouseWarpMode mouse_warp_mode) {
    mouse_warp_mode_ = mouse_warp_mode;
  }

  
  
  void ShowSharedEdgeIndicator(const aura::Window* from);
  void HideSharedEdgeIndicator();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;

 private:
  friend class DragWindowResizerTest;
  friend class MouseCursorEventFilterTest;
  FRIEND_TEST_ALL_PREFIXES(MouseCursorEventFilterTest, DoNotWarpTwice);
  FRIEND_TEST_ALL_PREFIXES(MouseCursorEventFilterTest, SetMouseWarpModeFlag);
  FRIEND_TEST_ALL_PREFIXES(MouseCursorEventFilterTest,
                           IndicatorBoundsTestOnRight);
  FRIEND_TEST_ALL_PREFIXES(MouseCursorEventFilterTest,
                           IndicatorBoundsTestOnLeft);
  FRIEND_TEST_ALL_PREFIXES(MouseCursorEventFilterTest,
                           IndicatorBoundsTestOnTopBottom);
  FRIEND_TEST_ALL_PREFIXES(DragWindowResizerTest, WarpMousePointer);

  void reset_was_mouse_warped_for_test() { was_mouse_warped_ = false; }

  
  
  
  
  
  bool WarpMouseCursorIfNecessary(aura::Window* target_root,
                                  const gfx::Point& point_in_screen);

  void UpdateHorizontalIndicatorWindowBounds();
  void UpdateVerticalIndicatorWindowBounds();

  MouseWarpMode mouse_warp_mode_;

  
  
  bool was_mouse_warped_;

  
  
  gfx::Rect src_indicator_bounds_;
  gfx::Rect dst_indicator_bounds_;

  
  const aura::Window* drag_source_root_;

  float scale_when_drag_started_;

  
  
  scoped_ptr<SharedDisplayEdgeIndicator> shared_display_edge_indicator_;

  DISALLOW_COPY_AND_ASSIGN(MouseCursorEventFilter);
};

}  
}  

#endif  
