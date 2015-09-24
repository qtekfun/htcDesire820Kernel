// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_DRAG_WINDOW_RESIZER_H_
#define ASH_WM_DRAG_WINDOW_RESIZER_H_

#include "ash/wm/window_resizer.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "ui/gfx/point.h"

namespace ash {
namespace internal {

class DragWindowController;
class TrayUser;

class ASH_EXPORT DragWindowResizer : public WindowResizer {
 public:
  virtual ~DragWindowResizer();

  
  
  
  static DragWindowResizer* Create(WindowResizer* next_window_resizer,
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
  FRIEND_TEST_ALL_PREFIXES(DragWindowResizerTest, DragWindowController);

  
  
  
  explicit DragWindowResizer(WindowResizer* next_window_resizer,
                             const Details& details);

  
  
  void UpdateDragWindow(const gfx::Rect& bounds, bool in_original_root);

  
  bool ShouldAllowMouseWarp();

  
  TrayUser* GetTrayUserItemAtPoint(const gfx::Point& point_in_screen);

  
  
  
  bool TryDraggingToNewUser();

  scoped_ptr<WindowResizer> next_window_resizer_;

  
  scoped_ptr<DragWindowController> drag_window_controller_;

  const Details details_;

  gfx::Point last_mouse_location_;

  
  static DragWindowResizer* instance_;

  base::WeakPtrFactory<DragWindowResizer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DragWindowResizer);
};

}  
}  

#endif  
