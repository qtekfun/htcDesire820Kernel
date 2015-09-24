// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_RESIZER_H_
#define ASH_WM_WINDOW_RESIZER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/client/window_move_client.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}

namespace ash {
namespace wm {
class WindowState;
}

class ASH_EXPORT WindowResizer {
 public:
  
  static const int kBoundsChange_None;
  static const int kBoundsChange_Repositions;
  static const int kBoundsChange_Resizes;

  
  static const int kBoundsChangeDirection_None;
  static const int kBoundsChangeDirection_Horizontal;
  static const int kBoundsChangeDirection_Vertical;

  WindowResizer();
  virtual ~WindowResizer();

  
  static int GetBoundsChangeForWindowComponent(int component);

  
  
  
  virtual void Drag(const gfx::Point& location, int event_flags) = 0;

  
  virtual void CompleteDrag(int event_flags) = 0;

  
  virtual void RevertDrag() = 0;

  
  virtual aura::Window* GetTarget() = 0;

  
  virtual const gfx::Point& GetInitialLocation() const = 0;

 protected:
  struct Details {
    Details();
    Details(aura::Window* window,
            const gfx::Point& location,
            int window_component,
            aura::client::WindowMoveSource source);
    ~Details();

    
    
    
    aura::Window* window;

    
    wm::WindowState* window_state;

    
    gfx::Rect initial_bounds_in_parent;

    
    
    gfx::Rect restore_bounds;

    
    gfx::Point initial_location_in_parent;

    
    float initial_opacity;

    
    int window_component;

    
    int bounds_change;

    
    int position_change_direction;

    
    int size_change_direction;

    
    bool is_resizable;

    
    aura::client::WindowMoveSource source;
  };

  static gfx::Rect CalculateBoundsForDrag(const Details& details,
                                          const gfx::Point& location);

  static gfx::Rect AdjustBoundsToGrid(const gfx::Rect& bounds,
                                      int grid_size);

  static bool IsBottomEdge(int component);

 private:
  
  
  static void AdjustDeltaForTouchResize(const Details& details,
                                        int* delta_x,
                                        int* delta_y);

  
  
  static gfx::Point GetOriginForDrag(const Details& details,
                                     int delta_x,
                                     int delta_y);

  
  static gfx::Size GetSizeForDrag(const Details& details,
                                  int* delta_x,
                                  int* delta_y);

  
  static int GetWidthForDrag(const Details& details,
                             int min_width,
                             int* delta_x);

  
  static int GetHeightForDrag(const Details& details,
                              int min_height,
                              int* delta_y);
};

ASH_EXPORT scoped_ptr<WindowResizer> CreateWindowResizer(
    aura::Window* window,
    const gfx::Point& point_in_parent,
    int window_component,
    aura::client::WindowMoveSource source);

}  

#endif  
