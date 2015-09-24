// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_WORKSPACE_LAYOUT_MANAGER_H_
#define ASH_WM_WORKSPACE_WORKSPACE_LAYOUT_MANAGER_H_

#include <set>

#include "ash/shell_observer.h"
#include "ash/wm/base_layout_manager.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/ui_base_types.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/rect.h"

namespace aura {
class RootWindow;
class Window;
}

namespace ui {
class Layer;
}

namespace ash {

namespace internal {

class ShelfLayoutManager;

class ASH_EXPORT WorkspaceLayoutManager : public BaseLayoutManager {
 public:
  explicit WorkspaceLayoutManager(aura::Window* window);
  virtual ~WorkspaceLayoutManager();

  void SetShelf(internal::ShelfLayoutManager* shelf);

  
  virtual void OnWindowResized() OVERRIDE {}
  virtual void OnWindowAddedToLayout(aura::Window* child) OVERRIDE;
  virtual void OnWillRemoveWindowFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnWindowRemovedFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnChildWindowVisibilityChanged(aura::Window* child,
                                              bool visibile) OVERRIDE;
  virtual void SetChildBounds(aura::Window* child,
                              const gfx::Rect& requested_bounds) OVERRIDE;

  
  virtual void OnDisplayWorkAreaInsetsChanged() OVERRIDE;

  
  virtual void OnWindowPropertyChanged(aura::Window* window,
                                       const void* key,
                                       intptr_t old) OVERRIDE;
  virtual void OnWindowStackingChanged(aura::Window* window) OVERRIDE;

  
  virtual void OnWindowShowTypeChanged(wm::WindowState* window_state,
                                       wm::WindowShowType old_type) OVERRIDE;

 private:
  
  virtual void ShowStateChanged(wm::WindowState* window_state,
                                ui::WindowShowState last_show_state) OVERRIDE;
  virtual void AdjustAllWindowsBoundsForWorkAreaChange(
      AdjustWindowReason reason) OVERRIDE;
  virtual void AdjustWindowBoundsForWorkAreaChange(
      wm::WindowState* window_state,
      AdjustWindowReason reason) OVERRIDE;

  void AdjustWindowBoundsWhenAdded(wm::WindowState* window_state);

  
  void UpdateShelfVisibility();

  
  
  void UpdateFullscreenState();

  
  
  void UpdateBoundsFromShowState(wm::WindowState* window_state,
                                 ui::WindowShowState last_show_state);

  
  
  bool SetMaximizedOrFullscreenBounds(wm::WindowState* window_state);

  
  
  void AdjustSnappedBounds(wm::WindowState* window_state, gfx::Rect* bounds);

  
  void SetChildBoundsAnimated(aura::Window* child, const gfx::Rect& bounds);

  internal::ShelfLayoutManager* shelf_;
  aura::Window* window_;

  
  
  gfx::Rect work_area_in_parent_;

  
  bool is_fullscreen_;

  DISALLOW_COPY_AND_ASSIGN(WorkspaceLayoutManager);
};

}  
}  

#endif  
