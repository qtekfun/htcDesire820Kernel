// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_DOCK_DOCKED_WINDOW_LAYOUT_MANAGER_OBSERVER_H_
#define UI_DOCK_DOCKED_WINDOW_LAYOUT_MANAGER_OBSERVER_H_

#include "ash/ash_export.h"

namespace gfx {
class Rect;
}

namespace ash {
namespace internal {

class ASH_EXPORT DockedWindowLayoutManagerObserver {
 public:
  
  
  enum Reason {
    CHILD_CHANGED,
    DISPLAY_RESIZED,
    DISPLAY_INSETS_CHANGED,
    SHELF_ALIGNMENT_CHANGED,
    KEYBOARD_BOUNDS_CHANGING
  };
  
  virtual void OnDockBoundsChanging(const gfx::Rect& new_bounds,
                                    Reason reason) = 0;

 protected:
  virtual ~DockedWindowLayoutManagerObserver() {}
};

}  
}  

#endif  
