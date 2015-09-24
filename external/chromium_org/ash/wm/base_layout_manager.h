// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_BASE_LAYOUT_MANAGER_H_
#define ASH_WM_BASE_LAYOUT_MANAGER_H_

#include <set>

#include "ash/ash_export.h"
#include "ash/shell_observer.h"
#include "ash/wm/window_state_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/layout_manager.h"
#include "ui/aura/window_observer.h"
#include "ui/base/ui_base_types.h"
#include "ui/events/event_handler.h"

namespace aura {
class Window;
}

namespace ash {
namespace wm {
class WindowState;
}

namespace internal {

class ASH_EXPORT BaseLayoutManager
    : public aura::LayoutManager,
      public aura::WindowObserver,
      public aura::client::ActivationChangeObserver,
      public ShellObserver,
      public wm::WindowStateObserver {
 public:
  typedef std::set<aura::Window*> WindowSet;

  explicit BaseLayoutManager(aura::Window* root_window);
  virtual ~BaseLayoutManager();

  const WindowSet& windows() const { return windows_; }

  
  
  
  static gfx::Rect BoundsWithScreenEdgeVisible(aura::Window* window,
                                               const gfx::Rect& restore_bounds);

  
  virtual void OnWindowResized() OVERRIDE;
  virtual void OnWindowAddedToLayout(aura::Window* child) OVERRIDE;
  virtual void OnWillRemoveWindowFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnWindowRemovedFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnChildWindowVisibilityChanged(aura::Window* child,
                                              bool visible) OVERRIDE;
  virtual void SetChildBounds(aura::Window* child,
                              const gfx::Rect& requested_bounds) OVERRIDE;

  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;
  virtual void OnWindowBoundsChanged(aura::Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

  
  virtual void OnDisplayWorkAreaInsetsChanged() OVERRIDE;

  
  virtual void OnWindowShowTypeChanged(wm::WindowState* window_state,
                                       wm::WindowShowType type) OVERRIDE;

 protected:
  enum AdjustWindowReason {
    ADJUST_WINDOW_DISPLAY_SIZE_CHANGED,
    ADJUST_WINDOW_WORK_AREA_INSETS_CHANGED,
  };

  
  virtual void ShowStateChanged(wm::WindowState* window_state,
                                ui::WindowShowState last_show_state);

  
  
  
  
  
  
  
  
  virtual void AdjustAllWindowsBoundsForWorkAreaChange(
      AdjustWindowReason reason);

  
  
  virtual void AdjustWindowBoundsForWorkAreaChange(
      wm::WindowState* window_state,
      AdjustWindowReason reason);

  aura::Window* root_window() { return root_window_; }

 private:
  
  void UpdateBoundsFromShowState(wm::WindowState* controller);

  
  WindowSet windows_;

  aura::Window* root_window_;

  DISALLOW_COPY_AND_ASSIGN(BaseLayoutManager);
};

}  
}  

#endif  
