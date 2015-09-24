// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELL_H_
#define ASH_SHELL_H_

#include <utility>
#include <vector>

#include "ash/ash_export.h"
#include "ash/metrics/user_metrics_recorder.h"
#include "ash/shelf/shelf_types.h"
#include "ash/system/user/login_status.h"
#include "ash/wm/system_modal_container_event_filter_delegate.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/window.h"
#include "ui/base/ui_base_types.h"
#include "ui/events/event_target.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/size.h"
#include "ui/views/corewm/cursor_manager.h"

class CommandLine;

namespace app_list {
class AppListView;
}
namespace aura {
class EventFilter;
class RootWindow;
class Window;
namespace client {
class ActivationClient;
class FocusClient;
class UserActionClient;
}
}
namespace chromeos {
class OutputConfigurator;
}

namespace gfx {
class ImageSkia;
class Point;
class Rect;
}

namespace keyboard {
class KeyboardController;
}

namespace ui {
class Layer;
}
namespace views {
class NonClientFrameView;
class Widget;
namespace corewm {
class CompoundEventFilter;
class InputMethodEventFilter;
class ShadowController;
class TooltipController;
class VisibilityController;
class WindowModalityController;
}
}

namespace ash {

class AcceleratorController;
class AccessibilityDelegate;
class AshNativeCursorManager;
class AutoclickController;
class CapsLockDelegate;
class DesktopBackgroundController;
class DisplayController;
class FirstRunHelper;
class HighContrastController;
class Launcher;
class ShelfDelegate;
class LockStateController;
class MagnificationController;
class MediaDelegate;
class MruWindowTracker;
class NestedDispatcherController;
class NewWindowDelegate;
class PartialMagnificationController;
class PowerButtonController;
class RootWindowHostFactory;
class ScreenAsh;
class SessionStateDelegate;
class ShelfItemDelegateManager;
class ShelfModel;
class ShellDelegate;
class ShellObserver;
class StickyKeys;
class SystemTray;
class SystemTrayDelegate;
class SystemTrayNotifier;
class UserActivityDetector;
class UserWallpaperDelegate;
class VideoDetector;
class WebNotificationTray;
class WindowCycleController;
class WindowPositioner;
class WindowSelectorController;

namespace internal {
class AcceleratorFilter;
class AppListController;
class CaptureController;
class DisplayChangeObserver;
class DisplayErrorObserver;
class DisplayManager;
class DragDropController;
class EventClientImpl;
class EventRewriterEventFilter;
class EventTransformationHandler;
class FocusCycler;
class KeyboardUMAEventFilter;
class LocaleNotificationController;
class MouseCursorEventFilter;
class OutputConfiguratorAnimation;
class OverlayEventFilter;
class PowerEventObserver;
class ResizeShadowController;
class ResolutionNotificationController;
class RootWindowController;
class ScopedTargetRootWindow;
class ScreenPositionController;
class ShelfWindowWatcher;
class SlowAnimationEventFilter;
class StatusAreaWidget;
class SystemGestureEventFilter;
class SystemModalContainerEventFilter;
class TouchObserverHUD;
class UserActivityNotifier;
class VideoActivityNotifier;
}

namespace shell {
class WindowWatcher;
}

namespace test {
class ShellTestApi;
}

class ASH_EXPORT Shell
    : public internal::SystemModalContainerEventFilterDelegate,
      public ui::EventTarget,
      public aura::client::ActivationChangeObserver {
 public:
  typedef std::vector<internal::RootWindowController*> RootWindowControllerList;

  enum Direction {
    FORWARD,
    BACKWARD
  };

  
  
  
  static Shell* CreateInstance(ShellDelegate* delegate);

  
  static Shell* GetInstance();

  
  static bool HasInstance();

  static void DeleteInstance();

  
  
  static internal::RootWindowController* GetPrimaryRootWindowController();

  
  
  static RootWindowControllerList GetAllRootWindowControllers();

  
  
  static aura::Window* GetPrimaryRootWindow();

  
  
  
  
  
  static aura::Window* GetTargetRootWindow();

  
  static gfx::Screen* GetScreen();

  
  static aura::Window::Windows GetAllRootWindows();

  static aura::Window* GetContainer(aura::Window* root_window,
                                    int container_id);
  static const aura::Window* GetContainer(const aura::Window* root_window,
                                          int container_id);

  
  
  
  static std::vector<aura::Window*> GetContainersFromAllRootWindows(
      int container_id,
      aura::Window* priority_root);

  void set_target_root_window(aura::Window* target_root_window) {
    target_root_window_ = target_root_window;
  }

  
  
  void ShowContextMenu(const gfx::Point& location_in_screen,
                       ui::MenuSourceType source_type);

  
  
  
  void ToggleAppList(aura::Window* anchor);

  
  bool GetAppListTargetVisibility() const;

  
  aura::Window* GetAppListWindow();

  
  app_list::AppListView* GetAppListView();

  
  bool IsSystemModalWindowOpen() const;

  
  void SimulateModalWindowOpenForTesting(bool modal_window_open) {
    simulate_modal_window_open_for_testing_ = modal_window_open;
  }

  
  
  views::NonClientFrameView* CreateDefaultNonClientFrameView(
      views::Widget* widget);

  
  void RotateFocus(Direction direction);

  
  
  
  
  void SetDisplayWorkAreaInsets(aura::Window* window,
                                const gfx::Insets& insets);

  
  void OnLoginStateChanged(user::LoginStatus status);

  
  void OnLoginUserProfilePrepared();

  
  
  void UpdateAfterLoginStatusChange(user::LoginStatus status);

  
  void OnAppTerminating();

  
  
  void OnLockStateChanged(bool locked);

  
  void CreateLauncher();

  
  
  void CreateKeyboard();

  
  void ShowLauncher();

  
  void AddShellObserver(ShellObserver* observer);
  void RemoveShellObserver(ShellObserver* observer);

  keyboard::KeyboardController* keyboard_controller() {
    return keyboard_controller_.get();
  }

  AcceleratorController* accelerator_controller() {
    return accelerator_controller_.get();
  }

  internal::DisplayManager* display_manager() {
    return display_manager_.get();
  }
  views::corewm::InputMethodEventFilter* input_method_filter() {
    return input_method_filter_.get();
  }
  views::corewm::CompoundEventFilter* env_filter() {
    return env_filter_.get();
  }
  views::corewm::TooltipController* tooltip_controller() {
    return tooltip_controller_.get();
  }
  internal::EventRewriterEventFilter* event_rewriter_filter() {
    return event_rewriter_filter_.get();
  }
  internal::OverlayEventFilter* overlay_filter() {
    return overlay_filter_.get();
  }
  DesktopBackgroundController* desktop_background_controller() {
    return desktop_background_controller_.get();
  }
  PowerButtonController* power_button_controller() {
    return power_button_controller_.get();
  }
  LockStateController* lock_state_controller() {
    return lock_state_controller_.get();
  }
  MruWindowTracker* mru_window_tracker() {
    return mru_window_tracker_.get();
  }
  UserActivityDetector* user_activity_detector() {
    return user_activity_detector_.get();
  }
  VideoDetector* video_detector() {
    return video_detector_.get();
  }
  WindowCycleController* window_cycle_controller() {
    return window_cycle_controller_.get();
  }
  WindowSelectorController* window_selector_controller() {
    return window_selector_controller_.get();
  }
  internal::FocusCycler* focus_cycler() {
    return focus_cycler_.get();
  }
  DisplayController* display_controller() {
    return display_controller_.get();
  }
  internal::MouseCursorEventFilter* mouse_cursor_filter() {
    return mouse_cursor_filter_.get();
  }
  internal::EventTransformationHandler* event_transformation_handler() {
    return event_transformation_handler_.get();
  }
  views::corewm::CursorManager* cursor_manager() { return &cursor_manager_; }

  ShellDelegate* delegate() { return delegate_.get(); }

  UserWallpaperDelegate* user_wallpaper_delegate() {
    return user_wallpaper_delegate_.get();
  }

  CapsLockDelegate* caps_lock_delegate() {
    return caps_lock_delegate_.get();
  }

  SessionStateDelegate* session_state_delegate() {
    return session_state_delegate_.get();
  }

  AccessibilityDelegate* accessibility_delegate() {
    return accessibility_delegate_.get();
  }

  NewWindowDelegate* new_window_delegate() {
    return new_window_delegate_.get();
  }

  MediaDelegate* media_delegate() {
    return media_delegate_.get();
  }

  HighContrastController* high_contrast_controller() {
    return high_contrast_controller_.get();
  }

  MagnificationController* magnification_controller() {
    return magnification_controller_.get();
  }

  PartialMagnificationController* partial_magnification_controller() {
    return partial_magnification_controller_.get();
  }

  AutoclickController* autoclick_controller() {
    return autoclick_controller_.get();
  }

  aura::client::ActivationClient* activation_client() {
    return activation_client_;
  }

  ShelfItemDelegateManager* shelf_item_delegate_manager() {
    return shelf_item_delegate_manager_.get();
  }

  ScreenAsh* screen() { return screen_; }

  
  void UpdateShelfVisibility();

  
  

  
  void SetShelfAutoHideBehavior(ShelfAutoHideBehavior behavior,
                                aura::Window* root_window);
  ShelfAutoHideBehavior GetShelfAutoHideBehavior(
      aura::Window* root_window) const;

  
  void SetShelfAlignment(ShelfAlignment alignment,
                         aura::Window* root_window);
  ShelfAlignment GetShelfAlignment(aura::Window* root_window);

  
  void SetDimming(bool should_dim);

  
  
  void NotifyFullscreenStateChange(bool is_fullscreen,
                                   aura::Window* root_window);

  
  
  void CreateModalBackground(aura::Window* window);

  
  
  
  void OnModalWindowRemoved(aura::Window* removed);

  
  WebNotificationTray* GetWebNotificationTray();

  
  bool HasPrimaryStatusArea();

  
  SystemTray* GetPrimarySystemTray();

  SystemTrayDelegate* system_tray_delegate() {
    return system_tray_delegate_.get();
  }

  SystemTrayNotifier* system_tray_notifier() {
    return system_tray_notifier_.get();
  }

  static void set_initially_hide_cursor(bool hide) {
    initially_hide_cursor_ = hide;
  }

  internal::ResizeShadowController* resize_shadow_controller() {
    return resize_shadow_controller_.get();
  }

  
  views::corewm::ShadowController* shadow_controller() {
    return shadow_controller_.get();
  }

  
  void DoInitialWorkspaceAnimation();

#if defined(OS_CHROMEOS) && defined(USE_X11)
  
  chromeos::OutputConfigurator* output_configurator() {
    return output_configurator_.get();
  }
  internal::OutputConfiguratorAnimation* output_configurator_animation() {
    return output_configurator_animation_.get();
  }
  internal::DisplayErrorObserver* display_error_observer() {
    return display_error_observer_.get();
  }
#endif  

  internal::ResolutionNotificationController*
      resolution_notification_controller() {
    return resolution_notification_controller_.get();
  }

  RootWindowHostFactory* root_window_host_factory() {
    return root_window_host_factory_.get();
  }

  ShelfModel* shelf_model() {
    return shelf_model_.get();
  }

  WindowPositioner* window_positioner() {
    return window_positioner_.get();
  }

  
  ShelfDelegate* GetShelfDelegate();

  UserMetricsRecorder* metrics() {
    return user_metrics_recorder_.get();
  }

  void SetTouchHudProjectionEnabled(bool enabled);

  bool is_touch_hud_projection_enabled() const {
    return is_touch_hud_projection_enabled_;
  }

#if defined(OS_CHROMEOS)
  
  
  ash::FirstRunHelper* CreateFirstRunHelper();

  StickyKeys* sticky_keys() {
    return sticky_keys_.get();
  }
#endif  

 private:
  FRIEND_TEST_ALL_PREFIXES(ExtendedDesktopTest, TestCursor);
  FRIEND_TEST_ALL_PREFIXES(WindowManagerTest, MouseEventCursors);
  FRIEND_TEST_ALL_PREFIXES(WindowManagerTest, TransformActivate);
  friend class internal::RootWindowController;
  friend class internal::ScopedTargetRootWindow;
  friend class test::ShellTestApi;
  friend class shell::WindowWatcher;

  typedef std::pair<aura::Window*, gfx::Rect> WindowAndBoundsPair;

  
  explicit Shell(ShellDelegate* delegate);
  virtual ~Shell();

  void Init();

  
  void InitKeyboard();

  
  void InitRootWindow(aura::Window* root_window);

  
  virtual bool CanWindowReceiveEvents(aura::Window* window) OVERRIDE;

  
  virtual bool CanAcceptEvent(const ui::Event& event) OVERRIDE;
  virtual EventTarget* GetParentTarget() OVERRIDE;
  virtual scoped_ptr<ui::EventTargetIterator> GetChildIterator() const OVERRIDE;
  virtual ui::EventTargeter* GetEventTargeter() OVERRIDE;
  virtual void OnEvent(ui::Event* event) OVERRIDE;

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

  static Shell* instance_;

  
  
  static bool initially_hide_cursor_;

  ScreenAsh* screen_;

  
  
  
  
  aura::Window* target_root_window_;
  aura::Window* scoped_target_root_window_;

  
  scoped_ptr<views::corewm::CompoundEventFilter> env_filter_;

  std::vector<WindowAndBoundsPair> to_restore_;

  scoped_ptr<UserMetricsRecorder> user_metrics_recorder_;
  scoped_ptr<keyboard::KeyboardController> keyboard_controller_;
  scoped_ptr<NestedDispatcherController> nested_dispatcher_controller_;
  scoped_ptr<AcceleratorController> accelerator_controller_;
  scoped_ptr<ShellDelegate> delegate_;
  scoped_ptr<SystemTrayDelegate> system_tray_delegate_;
  scoped_ptr<SystemTrayNotifier> system_tray_notifier_;
  scoped_ptr<UserWallpaperDelegate> user_wallpaper_delegate_;
  scoped_ptr<CapsLockDelegate> caps_lock_delegate_;
  scoped_ptr<SessionStateDelegate> session_state_delegate_;
  scoped_ptr<AccessibilityDelegate> accessibility_delegate_;
  scoped_ptr<NewWindowDelegate> new_window_delegate_;
  scoped_ptr<MediaDelegate> media_delegate_;
  scoped_ptr<ShelfDelegate> shelf_delegate_;
  scoped_ptr<ShelfItemDelegateManager> shelf_item_delegate_manager_;
  scoped_ptr<internal::ShelfWindowWatcher> shelf_window_watcher_;

  scoped_ptr<ShelfModel> shelf_model_;
  scoped_ptr<WindowPositioner> window_positioner_;

  scoped_ptr<internal::AppListController> app_list_controller_;

  scoped_ptr<internal::DragDropController> drag_drop_controller_;
  scoped_ptr<internal::ResizeShadowController> resize_shadow_controller_;
  scoped_ptr<views::corewm::ShadowController> shadow_controller_;
  scoped_ptr<views::corewm::VisibilityController> visibility_controller_;
  scoped_ptr<views::corewm::WindowModalityController>
      window_modality_controller_;
  scoped_ptr<views::corewm::TooltipController> tooltip_controller_;
  scoped_ptr<DesktopBackgroundController> desktop_background_controller_;
  scoped_ptr<PowerButtonController> power_button_controller_;
  scoped_ptr<LockStateController> lock_state_controller_;
  scoped_ptr<MruWindowTracker> mru_window_tracker_;
  scoped_ptr<UserActivityDetector> user_activity_detector_;
  scoped_ptr<VideoDetector> video_detector_;
  scoped_ptr<WindowCycleController> window_cycle_controller_;
  scoped_ptr<WindowSelectorController> window_selector_controller_;
  scoped_ptr<internal::FocusCycler> focus_cycler_;
  scoped_ptr<DisplayController> display_controller_;
  scoped_ptr<HighContrastController> high_contrast_controller_;
  scoped_ptr<MagnificationController> magnification_controller_;
  scoped_ptr<PartialMagnificationController> partial_magnification_controller_;
  scoped_ptr<AutoclickController> autoclick_controller_;
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<aura::client::UserActionClient> user_action_client_;
  aura::client::ActivationClient* activation_client_;
  scoped_ptr<internal::MouseCursorEventFilter> mouse_cursor_filter_;
  scoped_ptr<internal::ScreenPositionController> screen_position_controller_;
  scoped_ptr<internal::SystemModalContainerEventFilter> modality_filter_;
  scoped_ptr<internal::EventClientImpl> event_client_;
  scoped_ptr<internal::EventTransformationHandler>
      event_transformation_handler_;
  scoped_ptr<RootWindowHostFactory> root_window_host_factory_;

  
  scoped_ptr<internal::EventRewriterEventFilter> event_rewriter_filter_;

  
  
  scoped_ptr<internal::OverlayEventFilter> overlay_filter_;

  
  scoped_ptr<internal::KeyboardUMAEventFilter> keyboard_metrics_filter_;

  
  scoped_ptr<internal::SystemGestureEventFilter> system_gesture_filter_;

  
  scoped_ptr<internal::AcceleratorFilter> accelerator_filter_;

  
  scoped_ptr<views::corewm::InputMethodEventFilter> input_method_filter_;

  scoped_ptr<internal::DisplayManager> display_manager_;
  scoped_ptr<base::WeakPtrFactory<internal::DisplayManager> >
      weak_display_manager_factory_;

  scoped_ptr<internal::LocaleNotificationController>
      locale_notification_controller_;

#if defined(OS_CHROMEOS)
  scoped_ptr<internal::PowerEventObserver> power_event_observer_;
  scoped_ptr<internal::UserActivityNotifier> user_activity_notifier_;
  scoped_ptr<internal::VideoActivityNotifier> video_activity_notifier_;
  scoped_ptr<StickyKeys> sticky_keys_;
#if defined(USE_X11)
  
  scoped_ptr<chromeos::OutputConfigurator> output_configurator_;
  scoped_ptr<internal::OutputConfiguratorAnimation>
      output_configurator_animation_;
  scoped_ptr<internal::DisplayErrorObserver> display_error_observer_;

  
  scoped_ptr<internal::DisplayChangeObserver> display_change_observer_;
#endif  
#endif  

  scoped_ptr<internal::ResolutionNotificationController>
      resolution_notification_controller_;

  
  
  AshNativeCursorManager* native_cursor_manager_;
  views::corewm::CursorManager cursor_manager_;

  ObserverList<ShellObserver> observers_;

  
  bool simulate_modal_window_open_for_testing_;

  bool is_touch_hud_projection_enabled_;

  DISALLOW_COPY_AND_ASSIGN(Shell);
};

}  

#endif  
