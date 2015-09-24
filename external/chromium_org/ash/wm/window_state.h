// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_STATE_H_
#define ASH_WM_WINDOW_STATE_H_

#include "ash/ash_export.h"
#include "ash/wm/wm_types.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/aura/window_observer.h"
#include "ui/base/ui_base_types.h"

namespace aura {
class Window;
}

namespace gfx {
class Rect;
}

namespace ash {
class WindowResizer;

namespace wm {
class WindowStateDelegate;
class WindowStateObserver;

class ASH_EXPORT WindowState : public aura::WindowObserver {
 public:
  static bool IsMaximizedOrFullscreenState(ui::WindowShowState state);

  explicit WindowState(aura::Window* window);
  virtual ~WindowState();

  aura::Window* window() { return window_; }
  const aura::Window* window() const { return window_; }

  bool HasDelegate() const;
  void SetDelegate(scoped_ptr<WindowStateDelegate> delegate);

  
  ui::WindowShowState GetShowState() const;

  
  
  
  WindowShowType window_show_type() const { return window_show_type_; }

  
  bool IsMinimized() const;
  bool IsMaximized() const;
  bool IsFullscreen() const;
  bool IsMaximizedOrFullscreen() const;
  
  
  bool IsNormalShowState() const;
  bool IsActive() const;
  bool IsDocked() const;
  bool IsSnapped() const;

  
  bool CanMaximize() const;
  bool CanMinimize() const;
  bool CanResize() const;
  bool CanSnap() const;
  bool CanActivate() const;

  
  bool HasRestoreBounds() const;

  void Maximize();
  void Minimize();
  void Unminimize();
  void Activate();
  void Deactivate();
  void Restore();
  void ToggleMaximized();
  void ToggleFullscreen();
  void SnapLeft(const gfx::Rect& bounds);
  void SnapRight(const gfx::Rect& bounds);

  
  void SetBoundsInScreen(const gfx::Rect& bounds_in_screen);

  
  void SaveCurrentBoundsForRestore();

  
  
  gfx::Rect GetRestoreBoundsInParent() const;

  
  
  
  gfx::Rect GetRestoreBoundsInScreen() const;

  
  
  void SetRestoreBoundsInParent(const gfx::Rect& bounds_in_parent);

  
  
  void SetRestoreBoundsInScreen(const gfx::Rect& bounds_in_screen);

  
  void ClearRestoreBounds();

  
  
  
  
  
  bool always_restores_to_restore_bounds() const {
    return always_restores_to_restore_bounds_;
  }
  void set_always_restores_to_restore_bounds(bool value) {
    always_restores_to_restore_bounds_ = value;
  }

  
  
  bool hide_shelf_when_fullscreen() const {
    return hide_shelf_when_fullscreen_;
  }

  void set_hide_shelf_when_fullscreen(bool value) {
    hide_shelf_when_fullscreen_ = value;
  }

  
  
  bool animate_to_fullscreen() const {
    return animate_to_fullscreen_;
  }
  void set_animate_to_fullscreen(bool value) {
    animate_to_fullscreen_ = value;
  }

  
  
  
  
  
  bool minimum_visibility() const {
    return minimum_visibility_;
  }
  void set_minimum_visibility(bool minimum_visibility) {
    minimum_visibility_ = minimum_visibility;
  }

  
  
  const gfx::Rect* pre_auto_manage_window_bounds() const {
    return pre_auto_manage_window_bounds_.get();
  }
  void SetPreAutoManageWindowBounds(const gfx::Rect& bounds);

  

  void AddObserver(WindowStateObserver* observer);
  void RemoveObserver(WindowStateObserver* observer);

  
  bool is_dragged() const { return !!window_resizer_; }

  
  
  bool window_position_managed() const { return window_position_managed_; }
  void set_window_position_managed(bool window_position_managed) {
    window_position_managed_ = window_position_managed;
  }

  
  bool bounds_changed_by_user() const { return bounds_changed_by_user_; }
  void set_bounds_changed_by_user(bool bounds_changed_by_user) {
    bounds_changed_by_user_ = bounds_changed_by_user;
  }

  
  bool panel_attached() const {
    return panel_attached_;
  }
  void set_panel_attached(bool panel_attached) {
    panel_attached_ = panel_attached;
  }

  
  
  bool continue_drag_after_reparent() const {
    return continue_drag_after_reparent_;
  }
  void set_continue_drag_after_reparent(bool value) {
    continue_drag_after_reparent_ = value;
  }

  
  
  bool ignored_by_shelf() const { return ignored_by_shelf_; }
  void set_ignored_by_shelf(bool ignored_by_shelf) {
    ignored_by_shelf_ = ignored_by_shelf;
  }

  
  
  
  bool can_consume_system_keys() const { return can_consume_system_keys_; }
  void set_can_consume_system_keys(bool can_consume_system_keys) {
    can_consume_system_keys_ = can_consume_system_keys;
  }

  
  
  bool top_row_keys_are_function_keys() const {
    return top_row_keys_are_function_keys_;
  }
  void set_top_row_keys_are_function_keys(bool value) {
    top_row_keys_are_function_keys_ = value;
  }

  
  
  
  
  
  
  WindowResizer* window_resizer() const {
    return window_resizer_;
  }
  void set_window_resizer_(WindowResizer* window_resizer) {
    window_resizer_ = window_resizer;
  }

  
  virtual void OnWindowPropertyChanged(aura::Window* window,
                                       const void* key,
                                       intptr_t old) OVERRIDE;

 private:
  
  void SnapWindow(WindowShowType left_or_right,
                  const gfx::Rect& bounds);

  
  void SetWindowShowType(WindowShowType new_window_show_type);

  
  aura::Window* window_;
  scoped_ptr<WindowStateDelegate> delegate_;

  bool window_position_managed_;
  bool bounds_changed_by_user_;
  bool panel_attached_;
  bool continue_drag_after_reparent_;
  bool ignored_by_shelf_;
  bool can_consume_system_keys_;
  bool top_row_keys_are_function_keys_;
  WindowResizer* window_resizer_;

  bool always_restores_to_restore_bounds_;
  bool hide_shelf_when_fullscreen_;
  bool animate_to_fullscreen_;
  bool minimum_visibility_;

  
  
  
  scoped_ptr<gfx::Rect> pre_auto_manage_window_bounds_;

  ObserverList<WindowStateObserver> observer_list_;

  
  bool in_set_window_show_type_;

  WindowShowType window_show_type_;

  DISALLOW_COPY_AND_ASSIGN(WindowState);
};

ASH_EXPORT WindowState* GetActiveWindowState();

ASH_EXPORT WindowState* GetWindowState(aura::Window* window);

ASH_EXPORT const WindowState*
GetWindowState(const aura::Window* window);

}  
}  

#endif  
