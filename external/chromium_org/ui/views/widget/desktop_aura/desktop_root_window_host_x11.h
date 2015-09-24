// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_X11_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_X11_H_

#include <X11/extensions/shape.h>
#include <X11/Xlib.h>

#undef RootWindow

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "ui/aura/window_tree_host.h"
#include "ui/base/cursor/cursor_loader_x11.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/x/x11_atom_cache.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host.h"

namespace gfx {
class ImageSkia;
class ImageSkiaRep;
}

namespace views {
class DesktopDragDropClientAuraX11;
class DesktopDispatcherClient;
class DesktopRootWindowHostObserverX11;
class X11DesktopWindowMoveClient;
class X11WindowEventFilter;

class VIEWS_EXPORT DesktopRootWindowHostX11 :
    public DesktopRootWindowHost,
    public aura::RootWindowHost,
    public base::MessageLoop::Dispatcher {
 public:
  DesktopRootWindowHostX11(
      internal::NativeWidgetDelegate* native_widget_delegate,
      DesktopNativeWidgetAura* desktop_native_widget_aura);
  virtual ~DesktopRootWindowHostX11();

  
  static aura::Window* GetContentWindowForXID(XID xid);

  
  static DesktopRootWindowHostX11* GetHostForXID(XID xid);

  
  
  
  static std::vector<aura::Window*> GetAllOpenWindows();

  
  gfx::Rect GetX11RootWindowBounds() const;

  
  
  void HandleNativeWidgetActivationChanged(bool active);

  void AddObserver(views::DesktopRootWindowHostObserverX11* observer);
  void RemoveObserver(views::DesktopRootWindowHostObserverX11* observer);

  
  static void CleanUpWindowList();

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

private:
  
  
  void InitX11Window(const Widget::InitParams& params);

  
  
  aura::RootWindow* InitRootWindow(const Widget::InitParams& params);

  
  
  
  bool IsWindowManagerPresent();

  
  
  void SetWMSpecState(bool enabled, ::Atom state1, ::Atom state2);

  
  bool HasWMSpecProperty(const char* property) const;

  
  
  void OnCaptureReleased();

  
  
  
  void DispatchMouseEvent(ui::MouseEvent* event);

  
  
  
  void DispatchTouchEvent(ui::TouchEvent* event);

  
  void ResetWindowRegion();

  
  void SerializeImageRepresentation(const gfx::ImageSkiaRep& rep,
                                    std::vector<unsigned long>* data);

  
  static std::list<XID>& open_windows();

  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  base::WeakPtrFactory<DesktopRootWindowHostX11> close_widget_factory_;

  
  
  XDisplay* xdisplay_;
  ::Window xwindow_;

  
  ::Window x_root_window_;

  ui::X11AtomCache atom_cache_;

  
  bool window_mapped_;

  
  gfx::Rect bounds_;

  
  
  
  
  
  
  gfx::Rect previous_bounds_;

  
  gfx::Rect restored_bounds_;

  
  std::set< ::Atom> window_properties_;

  
  
  bool is_fullscreen_;

  
  bool is_always_on_top_;

  
  aura::RootWindow* root_window_;

  scoped_ptr<DesktopDispatcherClient> dispatcher_client_;

  DesktopDragDropClientAuraX11* drag_drop_client_;

  
  gfx::NativeCursor current_cursor_;

  scoped_ptr<X11WindowEventFilter> x11_window_event_filter_;
  scoped_ptr<X11DesktopWindowMoveClient> x11_window_move_client_;

  
  
  internal::NativeWidgetDelegate* native_widget_delegate_;

  DesktopNativeWidgetAura* desktop_native_widget_aura_;

  aura::Window* content_window_;

  
  
  DesktopRootWindowHostX11* window_parent_;
  std::set<DesktopRootWindowHostX11*> window_children_;

  ObserverList<DesktopRootWindowHostObserverX11> observer_list_;

  
  ::Region custom_window_shape_;

  
  
  
  
  
  static DesktopRootWindowHostX11* g_current_capture;

  
  
  static std::list<XID>* open_windows_;

  string16 window_title_;

  DISALLOW_COPY_AND_ASSIGN(DesktopRootWindowHostX11);
};

}  

#endif  
