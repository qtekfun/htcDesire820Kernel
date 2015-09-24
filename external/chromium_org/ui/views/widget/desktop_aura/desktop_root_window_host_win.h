// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_WIN_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_WIN_H_

#include "ui/aura/client/animation_host.h"
#include "ui/aura/window_tree_host.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host.h"
#include "ui/views/win/hwnd_message_handler_delegate.h"

namespace aura {
namespace client {
class DragDropClient;
class FocusClient;
}
}

namespace views {
class DesktopCursorClient;
class DesktopDragDropClientWin;
class HWNDMessageHandler;

namespace corewm {
class TooltipWin;
}

class VIEWS_EXPORT DesktopRootWindowHostWin
    : public DesktopRootWindowHost,
      public aura::client::AnimationHost,
      public aura::RootWindowHost,
      public HWNDMessageHandlerDelegate {
 public:
  DesktopRootWindowHostWin(
      internal::NativeWidgetDelegate* native_widget_delegate,
      DesktopNativeWidgetAura* desktop_native_widget_aura);
  virtual ~DesktopRootWindowHostWin();

  
  static aura::Window* GetContentWindowForHWND(HWND hwnd);

 protected:
  
  virtual void Init(aura::Window* content_window,
                    const Widget::InitParams& params,
                    aura::RootWindow::CreateParams* rw_create_params) OVERRIDE;
  virtual void OnRootWindowCreated(aura::RootWindow* root,
                                   const Widget::InitParams& params) OVERRIDE;
  virtual scoped_ptr<corewm::Tooltip> CreateTooltip() OVERRIDE;
  virtual scoped_ptr<aura::client::DragDropClient>
      CreateDragDropClient(DesktopNativeCursorManager* cursor_manager) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void CloseNow() OVERRIDE;
  virtual aura::RootWindowHost* AsRootWindowHost() OVERRIDE;
  virtual void ShowWindowWithState(ui::WindowShowState show_state) OVERRIDE;
  virtual void ShowMaximizedWithBounds(
      const gfx::Rect& restored_bounds) OVERRIDE;
  virtual bool IsVisible() const OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual void StackAtTop() OVERRIDE;
  virtual void CenterWindow(const gfx::Size& size) OVERRIDE;
  virtual void GetWindowPlacement(
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsInScreen() const OVERRIDE;
  virtual gfx::Rect GetClientAreaBoundsInScreen() const OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual gfx::Rect GetWorkAreaBoundsInScreen() const OVERRIDE;
  virtual void SetShape(gfx::NativeRegion native_region) OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual bool IsActive() const OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual bool HasCapture() const OVERRIDE;
  virtual void SetAlwaysOnTop(bool always_on_top) OVERRIDE;
  virtual bool IsAlwaysOnTop() const OVERRIDE;
  virtual bool SetWindowTitle(const string16& title) OVERRIDE;
  virtual void ClearNativeFocus() OVERRIDE;
  virtual Widget::MoveLoopResult RunMoveLoop(
      const gfx::Vector2d& drag_offset,
      Widget::MoveLoopSource source,
      Widget::MoveLoopEscapeBehavior escape_behavior) OVERRIDE;
  virtual void EndMoveLoop() OVERRIDE;
  virtual void SetVisibilityChangedAnimationsEnabled(bool value) OVERRIDE;
  virtual bool ShouldUseNativeFrame() OVERRIDE;
  virtual void FrameTypeChanged() OVERRIDE;
  virtual NonClientFrameView* CreateNonClientFrameView() OVERRIDE;
  virtual void SetFullscreen(bool fullscreen) OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual void SetOpacity(unsigned char opacity) OVERRIDE;
  virtual void SetWindowIcons(const gfx::ImageSkia& window_icon,
                              const gfx::ImageSkia& app_icon) OVERRIDE;
  virtual void InitModalType(ui::ModalType modal_type) OVERRIDE;
  virtual void FlashFrame(bool flash_frame) OVERRIDE;
  virtual void OnRootViewLayout() const OVERRIDE;
  virtual void OnNativeWidgetFocus() OVERRIDE;
  virtual void OnNativeWidgetBlur() OVERRIDE;
  virtual bool IsAnimatingClosed() const OVERRIDE;

  
  virtual aura::RootWindow* GetRootWindow() OVERRIDE;
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ToggleFullScreen() OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual void SetInsets(const gfx::Insets& insets) OVERRIDE;
  virtual gfx::Point GetLocationOnNativeScreen() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual bool QueryMouseLocation(gfx::Point* location_return) OVERRIDE;
  virtual bool ConfineCursorToRootWindow() OVERRIDE;
  virtual void UnConfineCursor() OVERRIDE;
  virtual void OnCursorVisibilityChanged(bool show) OVERRIDE;
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;
  virtual void PostNativeEvent(const base::NativeEvent& native_event) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void PrepareForShutdown() OVERRIDE;

  
  virtual void SetHostTransitionOffsets(
      const gfx::Vector2d& top_left_delta,
      const gfx::Vector2d& bottom_right_delta) OVERRIDE;
  virtual void OnWindowHidingAnimationCompleted() OVERRIDE;

  
  virtual bool IsWidgetWindow() const OVERRIDE;
  virtual bool IsUsingCustomFrame() const OVERRIDE;
  virtual void SchedulePaint() OVERRIDE;
  virtual void EnableInactiveRendering() OVERRIDE;
  virtual bool IsInactiveRenderingDisabled() OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual bool WidgetSizeIsClientSize() const OVERRIDE;
  virtual bool CanSaveFocus() const OVERRIDE;
  virtual void SaveFocusOnDeactivate() OVERRIDE;
  virtual void RestoreFocusOnActivate() OVERRIDE;
  virtual void RestoreFocusOnEnable() OVERRIDE;
  virtual bool IsModal() const OVERRIDE;
  virtual int GetInitialShowState() const OVERRIDE;
  virtual bool WillProcessWorkAreaChange() const OVERRIDE;
  virtual int GetNonClientComponent(const gfx::Point& point) const OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* path) OVERRIDE;
  virtual bool GetClientAreaInsets(gfx::Insets* insets) const OVERRIDE;
  virtual void GetMinMaxSize(gfx::Size* min_size,
                             gfx::Size* max_size) const OVERRIDE;
  virtual gfx::Size GetRootViewSize() const OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void PaintLayeredWindow(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;
  virtual bool ShouldHandleSystemCommands() const OVERRIDE;
  virtual InputMethod* GetInputMethod() OVERRIDE;
  virtual void HandleAppDeactivated() OVERRIDE;
  virtual void HandleActivationChanged(bool active) OVERRIDE;
  virtual bool HandleAppCommand(short command) OVERRIDE;
  virtual void HandleCancelMode() OVERRIDE;
  virtual void HandleCaptureLost() OVERRIDE;
  virtual void HandleClose() OVERRIDE;
  virtual bool HandleCommand(int command) OVERRIDE;
  virtual void HandleAccelerator(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void HandleCreate() OVERRIDE;
  virtual void HandleDestroying() OVERRIDE;
  virtual void HandleDestroyed() OVERRIDE;
  virtual bool HandleInitialFocus() OVERRIDE;
  virtual void HandleDisplayChange() OVERRIDE;
  virtual void HandleBeginWMSizeMove() OVERRIDE;
  virtual void HandleEndWMSizeMove() OVERRIDE;
  virtual void HandleMove() OVERRIDE;
  virtual void HandleWorkAreaChanged() OVERRIDE;
  virtual void HandleVisibilityChanging(bool visible) OVERRIDE;
  virtual void HandleVisibilityChanged(bool visible) OVERRIDE;
  virtual void HandleClientSizeChanged(const gfx::Size& new_size) OVERRIDE;
  virtual void HandleFrameChanged() OVERRIDE;
  virtual void HandleNativeFocus(HWND last_focused_window) OVERRIDE;
  virtual void HandleNativeBlur(HWND focused_window) OVERRIDE;
  virtual bool HandleMouseEvent(const ui::MouseEvent& event) OVERRIDE;
  virtual bool HandleKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual bool HandleUntranslatedKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void HandleTouchEvent(const ui::TouchEvent& event) OVERRIDE;
  virtual bool HandleIMEMessage(UINT message,
                                WPARAM w_param,
                                LPARAM l_param,
                                LRESULT* result) OVERRIDE;
  virtual void HandleInputLanguageChange(DWORD character_set,
                                         HKL input_language_id) OVERRIDE;
  virtual bool HandlePaintAccelerated(const gfx::Rect& invalid_rect) OVERRIDE;
  virtual void HandlePaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool HandleTooltipNotify(int w_param,
                                   NMHDR* l_param,
                                   LRESULT* l_result) OVERRIDE;
  virtual void HandleTooltipMouseMove(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) OVERRIDE;
  virtual bool PreHandleMSG(UINT message,
                            WPARAM w_param,
                            LPARAM l_param,
                            LRESULT* result) OVERRIDE;
  virtual void PostHandleMSG(UINT message,
                             WPARAM w_param,
                             LPARAM l_param) OVERRIDE;
  virtual bool HandleScrollEvent(const ui::ScrollEvent& event) OVERRIDE;

  Widget* GetWidget();
  const Widget* GetWidget() const;
  HWND GetHWND() const;

 private:
  void SetWindowTransparency();

  
  bool IsModalWindowActive() const;

  
  aura::RootWindow* root_window_;

  scoped_ptr<HWNDMessageHandler> message_handler_;
  scoped_ptr<aura::client::FocusClient> focus_client_;

  
  
  internal::NativeWidgetDelegate* native_widget_delegate_;

  DesktopNativeWidgetAura* desktop_native_widget_aura_;

  aura::Window* content_window_;

  
  DesktopDragDropClientWin* drag_drop_client_;

  
  
  
  gfx::Vector2d window_expansion_top_left_delta_;
  gfx::Vector2d window_expansion_bottom_right_delta_;

  
  
  gfx::Vector2d window_enlargement_;

  
  
  
  bool should_animate_window_close_;

  
  
  
  
  bool pending_close_;

  
  
  
  bool has_non_client_view_;

  
  
  corewm::TooltipWin* tooltip_;

  
  bool is_cursor_visible_;

  DISALLOW_COPY_AND_ASSIGN(DesktopRootWindowHostWin);
};

}  

#endif  
