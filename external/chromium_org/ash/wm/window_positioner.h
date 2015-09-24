// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_POSITIONER_H_
#define ASH_WM_WINDOW_POSITIONER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
class RootWindow;
}

namespace gfx {
class Display;
class Rect;
class Screen;
}

namespace ash {

namespace test {
class WindowPositionerTest;
}

class ASH_EXPORT WindowPositioner {
 public:
  
  
  static int GetForceMaximizedWidthLimit();

  
  
  static const int kDesktopBorderSize;

  
  static const int kMaximumWindowWidth;

  
  
  
  
  
  
  
  static void GetBoundsAndShowStateForNewWindow(
      const gfx::Screen* screen,
      const aura::Window* new_window,
      bool is_saved_bounds,
      ui::WindowShowState show_state_in,
      gfx::Rect* bounds_in_out,
      ui::WindowShowState* show_state_out);

  
  static gfx::Rect GetDefaultWindowBounds(const gfx::Display& display);

  
  
  
  static void RearrangeVisibleWindowOnHideOrRemove(
      const aura::Window* removed_window);

  
  
  static bool DisableAutoPositioning(bool ignore);

  
  
  
  static void RearrangeVisibleWindowOnShow(aura::Window* added_window);

  WindowPositioner();
  ~WindowPositioner();

  
  
  
  
  
  gfx::Rect GetPopupPosition(const gfx::Rect& old_pos);

  
  
  static void SetMaximizeFirstWindow(bool maximize);

 protected:
  friend class test::WindowPositionerTest;

  
  
  gfx::Rect SmartPopupPosition(const gfx::Rect& old_pos,
                               const gfx::Rect& work_area,
                               int grid);

  
  gfx::Rect NormalPopupPosition(const gfx::Rect& old_pos,
                                const gfx::Rect& work_area);

  
  gfx::Rect AlignPopupPosition(const gfx::Rect &pos,
                               const gfx::Rect &work_area,
                               int grid);

  
  static const int kMinimumWindowOffset;

  
  int pop_position_offset_increment_x;
  int pop_position_offset_increment_y;

  
  
  int popup_position_offset_from_screen_corner_x;
  int popup_position_offset_from_screen_corner_y;

  
  int last_popup_position_x_;
  int last_popup_position_y_;

  DISALLOW_COPY_AND_ASSIGN(WindowPositioner);
};

}  

#endif  
