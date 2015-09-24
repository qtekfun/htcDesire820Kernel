// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_SNAP_SIZER_H_
#define ASH_WM_WORKSPACE_SNAP_SIZER_H_

#include <vector>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/time/time.h"
#include "ui/gfx/rect.h"

namespace ash {
namespace wm {
class WindowState;
}

namespace internal {

class ASH_EXPORT SnapSizer {
 public:
  enum Edge {
    LEFT_EDGE,
    RIGHT_EDGE
  };

  enum InputType {
    TOUCH_MAXIMIZE_BUTTON_INPUT,
    OTHER_INPUT
  };

  
  
  
  SnapSizer(wm::WindowState* window_state,
            const gfx::Point& start,
            Edge edge,
            InputType input_type);
  virtual ~SnapSizer();

  
  static void SnapWindow(wm::WindowState* window_state, Edge edge);

  
  void SnapWindowToTargetBounds();

  
  void Update(const gfx::Point& location);

  
  const gfx::Rect& target_bounds() const { return target_bounds_; }

  
  
  gfx::Rect GetSnapBounds(const gfx::Rect& bounds);

  
  void SelectDefaultSizeAndDisableResize();

  
  
  gfx::Rect GetTargetBoundsForSize(size_t size_index) const;

  
  bool end_of_sequence() const { return end_of_sequence_; }

 private:
  
  
  
  
  int CalculateIncrement(int x, int reference_x) const;

  
  
  
  void ChangeBounds(int x, int delta);

  
  gfx::Rect GetTargetBounds() const;

  
  bool AlongEdge(int x) const;

  
  wm::WindowState* window_state_;

  const Edge edge_;

  
  gfx::Rect target_bounds_;

  
  base::TimeTicks time_last_update_;

  
  
  int size_index_;

  
  
  bool end_of_sequence_;

  
  
  bool resize_disabled_;

  
  int num_moves_since_adjust_;

  
  int last_adjust_x_;

  
  int last_update_x_;

  
  const int start_x_;

  
  
  
  const InputType input_type_;

  
  
  const std::vector<int> usable_width_;

  DISALLOW_COPY_AND_ASSIGN(SnapSizer);
};

}  
}  

#endif  
