// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_DOCK_DOCK_WINDOW_RESIZER_H_
#define ASH_WM_DOCK_DOCK_WINDOW_RESIZER_H_

#include "ash/wm/dock/dock_types.h"
#include "ash/wm/window_resizer.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"

namespace gfx {
class Point;
class Rect;
}

namespace aura {
class RootWindow;
}

namespace ash {
namespace internal {

class DockedWindowLayoutManager;

class ASH_EXPORT DockedWindowResizer : public WindowResizer {
 public:
  virtual ~DockedWindowResizer();

  
  
  
  static DockedWindowResizer* Create(WindowResizer* next_window_resizer,
                                     aura::Window* window,
                                     const gfx::Point& location,
                                     int window_component,
                                     aura::client::WindowMoveSource source);

  
  virtual void Drag(const gfx::Point& location, int event_flags) OVERRIDE;
  virtual void CompleteDrag(int event_flags) OVERRIDE;
  virtual void RevertDrag() OVERRIDE;
  virtual aura::Window* GetTarget() OVERRIDE;
  virtual const gfx::Point& GetInitialLocation() const OVERRIDE;

 private:
  
  
  
  DockedWindowResizer(WindowResizer* next_window_resizer,
                      const Details& details);

  
  
  void MaybeSnapToEdge(const gfx::Rect& bounds, gfx::Point* offset);

  
  
  void StartedDragging();

  
  
  void FinishedDragging();

  
  
  
  
  
  
  DockedAction MaybeReparentWindowOnDragCompletion(bool is_resized,
                                                   bool is_attached_panel);

  const Details details_;

  gfx::Point last_location_;

  
  scoped_ptr<WindowResizer> next_window_resizer_;

  
  internal::DockedWindowLayoutManager* dock_layout_;
  internal::DockedWindowLayoutManager* initial_dock_layout_;

  
  bool did_move_or_resize_;

  
  bool was_docked_;

  
  bool is_docked_;

  
  
  bool was_bounds_changed_by_user_;

  base::WeakPtrFactory<DockedWindowResizer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DockedWindowResizer);
};

}  
}  

#endif  
