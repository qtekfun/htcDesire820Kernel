// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_LAYOUT_MANAGER_H_
#define ASH_SHELF_SHELF_LAYOUT_MANAGER_H_

#include <vector>

#include "ash/ash_export.h"
#include "ash/launcher/launcher.h"
#include "ash/shelf/background_animator.h"
#include "ash/shelf/shelf_types.h"
#include "ash/shell_observer.h"
#include "ash/system/status_area_widget.h"
#include "ash/wm/dock/docked_window_layout_manager_observer.h"
#include "ash/wm/lock_state_observer.h"
#include "ash/wm/workspace/workspace_types.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/layout_manager.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/rect.h"
#include "ui/keyboard/keyboard_controller.h"
#include "ui/keyboard/keyboard_controller_observer.h"

namespace aura {
class RootWindow;
}

namespace ui {
class GestureEvent;
class ImplicitAnimationObserver;
}

namespace ash {
class ScreenAsh;
class ShelfLayoutManagerObserver;
class ShelfWidget;
FORWARD_DECLARE_TEST(WebNotificationTrayTest, PopupAndFullscreen);

namespace internal {

class PanelLayoutManagerTest;
class ShelfBezelEventFilter;
class ShelfLayoutManagerTest;
class StatusAreaWidget;
class WorkspaceController;

class ASH_EXPORT ShelfLayoutManager :
    public aura::LayoutManager,
    public ash::ShellObserver,
    public aura::client::ActivationChangeObserver,
    public DockedWindowLayoutManagerObserver,
    public keyboard::KeyboardControllerObserver,
    public LockStateObserver {
 public:

  
  
  static const int kWorkspaceAreaVisibleInset;

  
  
  static const int kWorkspaceAreaAutoHideInset;

  
  static const int kAutoHideSize;

  
  
  static const int kShelfSize;

  
  
  static const int kShelfItemInset;

  
  
  static int GetPreferredShelfSize();

  explicit ShelfLayoutManager(ShelfWidget* shelf);
  virtual ~ShelfLayoutManager();

  
  void SetAutoHideBehavior(ShelfAutoHideBehavior behavior);
  ShelfAutoHideBehavior auto_hide_behavior() const {
    return auto_hide_behavior_;
  }

  
  
  bool SetAlignment(ShelfAlignment alignment);
  
  
  
  ShelfAlignment GetAlignment() const;

  void set_workspace_controller(WorkspaceController* controller) {
    workspace_controller_ = controller;
  }

  bool updating_bounds() const { return updating_bounds_; }

  
  void PrepareForShutdown();

  
  
  bool IsVisible() const;

  
  gfx::Rect GetIdealBounds();

  
  const gfx::Rect& dock_bounds() const { return dock_bounds_; }

  
  
  void LayoutShelf();

  
  
  ShelfVisibilityState CalculateShelfVisibility();

  
  void UpdateVisibilityState();

  
  void UpdateAutoHideState();

  ShelfVisibilityState visibility_state() const {
    return state_.visibility_state;
  }
  ShelfAutoHideState auto_hide_state() const { return state_.auto_hide_state; }

  ShelfWidget* shelf_widget() { return shelf_; }

  
  
  void SetWindowOverlapsShelf(bool value);
  bool window_overlaps_shelf() const { return window_overlaps_shelf_; }

  void AddObserver(ShelfLayoutManagerObserver* observer);
  void RemoveObserver(ShelfLayoutManagerObserver* observer);

  
  void StartGestureDrag(const ui::GestureEvent& gesture);
  enum DragState {
    DRAG_SHELF,
    DRAG_TRAY
  };
  
  
  
  DragState UpdateGestureDrag(const ui::GestureEvent& gesture);
  void CompleteGestureDrag(const ui::GestureEvent& gesture);
  void CancelGestureDrag();

  
  virtual void OnWindowResized() OVERRIDE;
  virtual void OnWindowAddedToLayout(aura::Window* child) OVERRIDE;
  virtual void OnWillRemoveWindowFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnWindowRemovedFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnChildWindowVisibilityChanged(aura::Window* child,
                                              bool visible) OVERRIDE;
  virtual void SetChildBounds(aura::Window* child,
                              const gfx::Rect& requested_bounds) OVERRIDE;

  
  virtual void OnLockStateChanged(bool locked) OVERRIDE;

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

  
  virtual void OnLockStateEvent(LockStateObserver::EventType event) OVERRIDE;

  
  
  
  
  template<typename T>
  T SelectValueForShelfAlignment(T bottom, T left, T right, T top) const {
    switch (GetAlignment()) {
      case SHELF_ALIGNMENT_BOTTOM:
        return bottom;
      case SHELF_ALIGNMENT_LEFT:
        return left;
      case SHELF_ALIGNMENT_RIGHT:
        return right;
      case SHELF_ALIGNMENT_TOP:
        return top;
    }
    NOTREACHED();
    return right;
  }

  template<typename T>
  T PrimaryAxisValue(T horizontal, T vertical) const {
    return IsHorizontalAlignment() ? horizontal : vertical;
  }

  
  bool IsHorizontalAlignment() const;

  
  
  static ShelfLayoutManager* ForLauncher(aura::Window* window);

 private:
  class AutoHideEventFilter;
  class UpdateShelfObserver;
  friend class ash::ScreenAsh;
  friend class PanelLayoutManagerTest;
  friend class ShelfLayoutManagerTest;
  FRIEND_TEST_ALL_PREFIXES(ash::WebNotificationTrayTest, PopupAndFullscreen);

  struct TargetBounds {
    TargetBounds();
    ~TargetBounds();

    float opacity;
    float status_opacity;
    gfx::Rect shelf_bounds_in_root;
    gfx::Rect launcher_bounds_in_shelf;
    gfx::Rect status_bounds_in_shelf;
    gfx::Insets work_area_insets;
  };

  struct State {
    State() : visibility_state(SHELF_VISIBLE),
              auto_hide_state(SHELF_AUTO_HIDE_HIDDEN),
              window_state(WORKSPACE_WINDOW_STATE_DEFAULT),
              is_screen_locked(false) {}

    
    
    
    
    bool Equals(const State& other) const {
      return other.visibility_state == visibility_state &&
          (visibility_state != SHELF_AUTO_HIDE ||
           other.auto_hide_state == auto_hide_state) &&
          other.window_state == window_state &&
          other.is_screen_locked == is_screen_locked;
    }

    ShelfVisibilityState visibility_state;
    ShelfAutoHideState auto_hide_state;
    WorkspaceWindowState window_state;
    bool is_screen_locked;
  };

  
  void SetState(ShelfVisibilityState visibility_state);

  
  
  
  void UpdateBoundsAndOpacity(const TargetBounds& target_bounds,
                              bool animate,
                              ui::ImplicitAnimationObserver* observer);

  
  void StopAnimating();

  
  
  void GetShelfSize(int* width, int* height);

  
  void AdjustBoundsBasedOnAlignment(int inset, gfx::Rect* bounds) const;

  
  void CalculateTargetBounds(const State& state, TargetBounds* target_bounds);

  
  
  void UpdateTargetBoundsForGesture(TargetBounds* target_bounds) const;

  
  void UpdateShelfBackground(BackgroundAnimatorChangeType type);

  
  ShelfBackgroundType GetShelfBackgroundType() const;

  
  void UpdateAutoHideStateNow();

  
  
  void StopAutoHideTimer();

  
  
  
  
  gfx::Rect GetAutoHideShowShelfRegionInScreen() const;

  
  
  ShelfAutoHideState CalculateAutoHideState(
      ShelfVisibilityState visibility_state) const;

  
  void UpdateHitTestBounds();

  
  bool IsShelfWindow(aura::Window* window);

  int GetWorkAreaSize(const State& state, int size) const;

  
  
  gfx::Rect GetAvailableBounds() const;

  
  virtual void OnKeyboardBoundsChanging(
      const gfx::Rect& keyboard_bounds) OVERRIDE;

  
  virtual void OnDockBoundsChanging(
      const gfx::Rect& dock_bounds,
      DockedWindowLayoutManagerObserver::Reason reason) OVERRIDE;

  
  gfx::Insets GetInsetsForAlignment(int distance) const;

  
  
  
  aura::Window* root_window_;

  
  
  bool updating_bounds_;

  
  ShelfAutoHideBehavior auto_hide_behavior_;

  
  ShelfAlignment alignment_;

  
  State state_;

  ShelfWidget* shelf_;

  WorkspaceController* workspace_controller_;

  
  bool window_overlaps_shelf_;

  base::OneShotTimer<ShelfLayoutManager> auto_hide_timer_;

  
  
  bool mouse_over_shelf_when_auto_hide_timer_started_;

  
  
  scoped_ptr<AutoHideEventFilter> auto_hide_event_filter_;

  
  scoped_ptr<ShelfBezelEventFilter> bezel_event_filter_;

  ObserverList<ShelfLayoutManagerObserver> observers_;

  
  
  
  
  enum GestureDragStatus {
    GESTURE_DRAG_NONE,
    GESTURE_DRAG_IN_PROGRESS,
    GESTURE_DRAG_CANCEL_IN_PROGRESS,
    GESTURE_DRAG_COMPLETE_IN_PROGRESS
  };
  GestureDragStatus gesture_drag_status_;

  
  
  float gesture_drag_amount_;

  
  ShelfAutoHideState gesture_drag_auto_hide_state_;

  
  UpdateShelfObserver* update_shelf_observer_;

  
  gfx::Rect keyboard_bounds_;

  
  gfx::Rect dock_bounds_;

  DISALLOW_COPY_AND_ASSIGN(ShelfLayoutManager);
};

}  
}  

#endif  
