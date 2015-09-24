// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_DOCK_DOCKED_WINDOW_LAYOUT_MANAGER_H_
#define ASH_WM_DOCK_DOCKED_WINDOW_LAYOUT_MANAGER_H_

#include "ash/ash_export.h"
#include "ash/shelf/shelf_layout_manager_observer.h"
#include "ash/shell_observer.h"
#include "ash/wm/dock/dock_types.h"
#include "ash/wm/dock/docked_window_layout_manager_observer.h"
#include "ash/wm/window_state_observer.h"
#include "ash/wm/workspace/snap_types.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/layout_manager.h"
#include "ui/aura/window.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/rect.h"
#include "ui/keyboard/keyboard_controller_observer.h"

namespace aura {
class Window;
}

namespace gfx {
class Point;
}

namespace views {
class Widget;
}

namespace ash {
class Launcher;

namespace internal {
class DockedBackgroundWidget;
class DockedWindowLayoutManagerObserver;
class DockedWindowResizerTest;
class ShelfLayoutManager;
class WorkspaceController;

struct WindowWithHeight {
  explicit WindowWithHeight(aura::Window* window) :
    window_(window),
    height_(window->bounds().height()) { }
  aura::Window* window() { return window_; }
  const aura::Window* window() const { return window_; }
  aura::Window* window_;
  int height_;
};

class ASH_EXPORT DockedWindowLayoutManager
    : public aura::LayoutManager,
      public ash::ShellObserver,
      public aura::WindowObserver,
      public aura::client::ActivationChangeObserver,
      public keyboard::KeyboardControllerObserver,
      public ShelfLayoutManagerObserver,
      public wm::WindowStateObserver {
 public:
  
  static const int kMaxDockWidth;

  
  static const int kMinDockWidth;

  DockedWindowLayoutManager(aura::Window* dock_container,
                            WorkspaceController* workspace_controller);
  virtual ~DockedWindowLayoutManager();

  
  void Shutdown();

  
  virtual void AddObserver(DockedWindowLayoutManagerObserver* observer);
  virtual void RemoveObserver(DockedWindowLayoutManagerObserver* observer);

  
  
  void StartDragging(aura::Window* window);

  
  void DockDraggedWindow(aura::Window* window);

  
  void UndockDraggedWindow();

  
  
  
  void FinishDragging(DockedAction action, DockedActionSource source);

  ash::Launcher* launcher() { return launcher_; }
  void SetLauncher(ash::Launcher* launcher);

  
  DockedAlignment GetAlignmentOfWindow(const aura::Window* window) const;

  
  DockedAlignment CalculateAlignment() const;

  
  
  bool CanDockWindow(aura::Window* window, SnapType edge);

  aura::Window* dock_container() const { return dock_container_; }

  
  const gfx::Rect& docked_bounds() const { return docked_bounds_; }

  
  const gfx::Rect dragged_bounds() const { return dragged_bounds_;}

  
  bool is_dragged_window_docked() const { return is_dragged_window_docked_; }

  
  void OnShelfBoundsChanged();

  
  virtual void OnWindowResized() OVERRIDE;
  virtual void OnWindowAddedToLayout(aura::Window* child) OVERRIDE;
  virtual void OnWillRemoveWindowFromLayout(aura::Window* child) OVERRIDE {}
  virtual void OnWindowRemovedFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnChildWindowVisibilityChanged(aura::Window* child,
                                              bool visibile) OVERRIDE;
  virtual void SetChildBounds(aura::Window* child,
                              const gfx::Rect& requested_bounds) OVERRIDE;

  
  virtual void OnDisplayWorkAreaInsetsChanged() OVERRIDE;
  virtual void OnFullscreenStateChanged(bool is_fullscreen,
                                        aura::Window* root_window) OVERRIDE;
  virtual void OnShelfAlignmentChanged(aura::Window* root_window) OVERRIDE;

  
  virtual void OnBackgroundUpdated(
      ShelfBackgroundType background_type,
      BackgroundAnimatorChangeType change_type) OVERRIDE;

  
  virtual void OnWindowShowTypeChanged(wm::WindowState* window_state,
                                       wm::WindowShowType old_type) OVERRIDE;

  
  virtual void OnWindowBoundsChanged(aura::Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;
  virtual void OnWindowVisibilityChanging(aura::Window* window,
                                          bool visible) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

 private:
  class ShelfWindowObserver;
  friend class DockedWindowLayoutManagerTest;
  friend class DockedWindowResizerTest;

  
  static const int kMinDockGap;

  
  static const int kIdealWidth;

  
  
  void MaybeMinimizeChildrenExcept(aura::Window* child);

  
  void MinimizeDockedWindow(wm::WindowState* window_state);
  void RestoreDockedWindow(wm::WindowState* window_state);

  
  void RecordUmaAction(DockedAction action, DockedActionSource source);

  
  void UpdateDockedWidth(int width);

  
  void OnDraggedWindowDocked(aura::Window* window);

  
  void OnDraggedWindowUndocked();

  
  bool IsAnyWindowDocked();

  
  void Relayout();

  
  
  
  
  
  int CalculateWindowHeightsAndRemainingRoom(
      const gfx::Rect work_area,
      std::vector<WindowWithHeight>* visible_windows);

  
  
  int CalculateIdealWidth(const std::vector<WindowWithHeight>& visible_windows);

  
  
  
  void FanOutChildren(const gfx::Rect& work_area,
                      int ideal_docked_width,
                      int available_room,
                      std::vector<WindowWithHeight>* visible_windows);

  
  
  
  void UpdateDockBounds(DockedWindowLayoutManagerObserver::Reason reason);

  
  
  void UpdateStacking(aura::Window* active_window);

  
  virtual void OnKeyboardBoundsChanging(
      const gfx::Rect& keyboard_bounds) OVERRIDE;

  
  aura::Window* dock_container_;
  
  bool in_layout_;

  
  
  
  
  aura::Window* dragged_window_;

  
  bool is_dragged_window_docked_;

  
  
  
  bool is_dragged_from_dock_;

  
  Launcher* launcher_;

  
  WorkspaceController* workspace_controller_;
  
  bool in_fullscreen_;
  
  int docked_width_;

  
  gfx::Rect docked_bounds_;

  
  gfx::Rect dragged_bounds_;

  
  DockedAlignment alignment_;

  
  
  aura::Window* last_active_window_;

  
  
  base::Time last_action_time_;

  
  scoped_ptr<ShelfWindowObserver> shelf_observer_;

  
  scoped_ptr<DockedBackgroundWidget> background_widget_;

  
  ObserverList<DockedWindowLayoutManagerObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(DockedWindowLayoutManager);
};

}  
}  

#endif  
