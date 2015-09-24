// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ROOT_WINDOW_CONTROLLER_H_
#define ASH_ROOT_WINDOW_CONTROLLER_H_

#include <map>

#include "ash/ash_export.h"
#include "ash/shelf/shelf_types.h"
#include "ash/shell_observer.h"
#include "ash/system/user/login_status.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"
#include "ui/base/ui_base_types.h"

class SkBitmap;

namespace aura {
class EventFilter;
class Window;
}

namespace gfx {
class Point;
}

namespace views {
class Widget;

namespace corewm {
class InputMethodEventFilter;
class RootWindowEventFilter;
class ScopedCaptureClient;
}
}

namespace keyboard {
class KeyboardController;
}

namespace ash {
class ShelfWidget;
class SoloWindowTracker;
class StackingController;
class SystemTray;
class ToplevelWindowEventHandler;

namespace internal {

class AlwaysOnTopController;
class AnimatingDesktopController;
class DesktopBackgroundWidgetController;
class DockedWindowLayoutManager;
class PanelLayoutManager;
class RootWindowLayoutManager;
class ScreenDimmer;
class ShelfLayoutManager;
class StatusAreaWidget;
class SystemBackgroundController;
class SystemModalContainerLayoutManager;
class TouchHudDebug;
class TouchHudProjection;
class WorkspaceController;

#if defined(OS_CHROMEOS)
class BootSplashScreen;
#endif

class ASH_EXPORT RootWindowController : public ShellObserver {
 public:

  
  static void CreateForPrimaryDisplay(aura::RootWindow* root_window);

  
  static void CreateForSecondaryDisplay(aura::RootWindow* root_window);

  
  
  static void CreateForVirtualKeyboardDisplay(aura::RootWindow* root_window);

  
  
  
  
  static RootWindowController* ForLauncher(aura::Window* window);

  
  static RootWindowController* ForWindow(const aura::Window* window);

  
  static internal::RootWindowController* ForTargetRootWindow();

  
  static aura::Window* GetContainerForWindow(aura::Window* window);

  virtual ~RootWindowController();

  aura::Window* root_window() { return dispatcher()->window(); }
  aura::WindowEventDispatcher* dispatcher() { return root_window_.get(); }

  RootWindowLayoutManager* root_window_layout() { return root_window_layout_; }

  WorkspaceController* workspace_controller() {
    return workspace_controller_.get();
  }

  AlwaysOnTopController* always_on_top_controller() {
    return always_on_top_controller_.get();
  }

  ScreenDimmer* screen_dimmer() { return screen_dimmer_.get(); }

  
  
  ShelfWidget* shelf() { return shelf_.get(); }

  
  TouchHudDebug* touch_hud_debug() const {
    return touch_hud_debug_;
  }
  TouchHudProjection* touch_hud_projection() const {
    return touch_hud_projection_;
  }

  
  
  
  
  void set_touch_hud_debug(TouchHudDebug* hud) {
    touch_hud_debug_ = hud;
  }
  void set_touch_hud_projection(TouchHudProjection* hud) {
    touch_hud_projection_ = hud;
  }

  DesktopBackgroundWidgetController* wallpaper_controller() {
    return wallpaper_controller_.get();
  }
  void SetWallpaperController(DesktopBackgroundWidgetController* controller);
  AnimatingDesktopController* animating_wallpaper_controller() {
    return animating_wallpaper_controller_.get();
  }
  void SetAnimatingWallpaperController(AnimatingDesktopController* controller);

  SoloWindowTracker* solo_window_tracker() {
    return solo_window_tracker_.get();
  }

  
  
  ShelfLayoutManager* GetShelfLayoutManager();

  
  
  
  SystemTray* GetSystemTray();

  
  
  void ShowContextMenu(const gfx::Point& location_in_screen,
                       ui::MenuSourceType source_type);

  
  
  
  
  
  
  
  SystemModalContainerLayoutManager* GetSystemModalLayoutManager(
      aura::Window* window);

  aura::Window* GetContainer(int container_id);
  const aura::Window* GetContainer(int container_id) const;

  
  void ShowLauncher();

  
  void OnLauncherCreated();

  
  
  void UpdateAfterLoginStatusChange(user::LoginStatus status);

  
  
  
  void HandleInitialDesktopBackgroundAnimationStarted();

  
  
  
  
  void OnWallpaperAnimationFinished(views::Widget* widget);

  
  
  
  
  void Shutdown();

  
  void CloseChildWindows();

  
  void MoveWindowsTo(aura::Window* dest);

  
  void UpdateShelfVisibility();

  
  void InitTouchHuds();

  
  
  const aura::Window* GetWindowForFullscreenMode() const;

  
  void ActivateKeyboard(keyboard::KeyboardController* keyboard_controller);

  
  void DeactivateKeyboard(keyboard::KeyboardController* keyboard_controller);

 private:
  explicit RootWindowController(aura::RootWindow* root_window);
  enum RootWindowType {
    PRIMARY,
    SECONDARY,
    VIRTUAL_KEYBOARD
  };

  
  
  
  void Init(RootWindowType root_window_type, bool first_run_after_boot);

  void InitLayoutManagers();

  
  
  void CreateSystemBackground(bool is_first_run_after_boot);

  
  
  void CreateContainersInRootWindow(aura::Window* root_window);

  
  void EnableTouchHudProjection();

  
  void DisableTouchHudProjection();

  
  virtual void OnLoginStateChanged(user::LoginStatus status) OVERRIDE;
  virtual void OnTouchHudProjectionToggled(bool enabled) OVERRIDE;

  scoped_ptr<aura::RootWindow> root_window_;
  RootWindowLayoutManager* root_window_layout_;

  scoped_ptr<StackingController> stacking_controller_;

  
  scoped_ptr<ShelfWidget> shelf_;

  
  
  
  
  
  scoped_ptr<aura::Window> mouse_event_target_;

  
  DockedWindowLayoutManager* docked_layout_manager_;

  
  PanelLayoutManager* panel_layout_manager_;

  scoped_ptr<SystemBackgroundController> system_background_;
#if defined(OS_CHROMEOS)
  scoped_ptr<BootSplashScreen> boot_splash_screen_;
#endif

  scoped_ptr<ScreenDimmer> screen_dimmer_;
  scoped_ptr<WorkspaceController> workspace_controller_;
  scoped_ptr<AlwaysOnTopController> always_on_top_controller_;

  
  
  TouchHudDebug* touch_hud_debug_;
  TouchHudProjection* touch_hud_projection_;

  
  scoped_ptr<ToplevelWindowEventHandler> default_container_handler_;
  scoped_ptr<ToplevelWindowEventHandler> always_on_top_container_handler_;
  scoped_ptr<ToplevelWindowEventHandler> modal_container_handler_;
  scoped_ptr<ToplevelWindowEventHandler> lock_modal_container_handler_;
  scoped_ptr<ToplevelWindowEventHandler> panel_container_handler_;
  scoped_ptr<ToplevelWindowEventHandler> docked_container_handler_;

  scoped_ptr<DesktopBackgroundWidgetController> wallpaper_controller_;
  scoped_ptr<AnimatingDesktopController> animating_wallpaper_controller_;
  scoped_ptr<views::corewm::ScopedCaptureClient> capture_client_;
  scoped_ptr<SoloWindowTracker> solo_window_tracker_;

  DISALLOW_COPY_AND_ASSIGN(RootWindowController);
};


ASH_EXPORT RootWindowController* GetRootWindowController(
    const aura::Window* root_window);

}  
}  

#endif  
