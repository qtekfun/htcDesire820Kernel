// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_DEFAULT_WINDOW_RESIZER_H_
#define ASH_WM_DEFAULT_WINDOW_RESIZER_H_

#include "ash/wm/window_resizer.h"
#include "base/compiler_specific.h"

namespace ash {

class ASH_EXPORT DefaultWindowResizer : public WindowResizer {
 public:
  virtual ~DefaultWindowResizer();

  
  
  static DefaultWindowResizer* Create(aura::Window* window,
                                      const gfx::Point& location,
                                      int window_component,
                                      aura::client::WindowMoveSource source);

  
  bool is_resizable() const { return details_.is_resizable; }

  bool changed_size() const {
    return !(details_.bounds_change & kBoundsChange_Repositions);
  }
  aura::Window* target_window() const { return details_.window; }

  
  virtual void Drag(const gfx::Point& location, int event_flags) OVERRIDE;
  virtual void CompleteDrag(int event_flags) OVERRIDE;
  virtual void RevertDrag() OVERRIDE;
  virtual aura::Window* GetTarget() OVERRIDE;
  virtual const gfx::Point& GetInitialLocation() const OVERRIDE;

 private:
  explicit DefaultWindowResizer(const Details& details);

  const Details details_;

  
  bool did_move_or_resize_;

  DISALLOW_COPY_AND_ASSIGN(DefaultWindowResizer);
};

}  

#endif  
