// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_PRIVATE_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_PRIVATE_H_

#include "base/strings/string16.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/ime/input_method_delegate.h"
#include "ui/views/widget/native_widget.h"

namespace gfx {
class ImageSkia;
class Rect;
}

namespace ui {
class NativeTheme;
class OSExchangeData;
}

namespace views {
class InputMethod;
class TooltipManager;
namespace internal {

class VIEWS_EXPORT NativeWidgetPrivate : public NativeWidget {
 public:
  virtual ~NativeWidgetPrivate() {}

  
  
  static NativeWidgetPrivate* CreateNativeWidget(
      internal::NativeWidgetDelegate* delegate);

  static NativeWidgetPrivate* GetNativeWidgetForNativeView(
      gfx::NativeView native_view);
  static NativeWidgetPrivate* GetNativeWidgetForNativeWindow(
      gfx::NativeWindow native_window);

  
  
  static NativeWidgetPrivate* GetTopLevelNativeWidget(
      gfx::NativeView native_view);

  static void GetAllChildWidgets(gfx::NativeView native_view,
                                 Widget::Widgets* children);
  static void GetAllOwnedWidgets(gfx::NativeView native_view,
                                 Widget::Widgets* owned);
  static void ReparentNativeView(gfx::NativeView native_view,
                                 gfx::NativeView new_parent);

  
  static bool IsMouseButtonDown();

  
  static bool IsTouchDown();

  
  virtual void InitNativeWidget(const Widget::InitParams& params) = 0;

  
  
  virtual NonClientFrameView* CreateNonClientFrameView() = 0;

  virtual bool ShouldUseNativeFrame() const = 0;
  virtual void FrameTypeChanged() = 0;

  
  
  virtual Widget* GetWidget() = 0;
  virtual const Widget* GetWidget() const = 0;

  
  virtual gfx::NativeView GetNativeView() const = 0;
  virtual gfx::NativeWindow GetNativeWindow() const = 0;

  
  virtual Widget* GetTopLevelWidget() = 0;

  
  
  virtual const ui::Compositor* GetCompositor() const = 0;
  virtual ui::Compositor* GetCompositor() = 0;

  
  virtual ui::Layer* GetLayer() = 0;

  
  
  
  
  
  
  
  virtual void ReorderNativeViews() = 0;

  
  
  virtual void ViewRemoved(View* view) = 0;

  
  
  
  virtual void SetNativeWindowProperty(const char* name, void* value) = 0;
  virtual void* GetNativeWindowProperty(const char* name) const = 0;

  
  
  virtual TooltipManager* GetTooltipManager() const = 0;

  
  virtual void SetCapture() = 0;
  virtual void ReleaseCapture() = 0;

  
  virtual bool HasCapture() const = 0;

  
  
  
  
  virtual InputMethod* CreateInputMethod() = 0;

  
  virtual InputMethodDelegate* GetInputMethodDelegate() = 0;


  
  virtual void CenterWindow(const gfx::Size& size) = 0;

  
  
  virtual void GetWindowPlacement(
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const = 0;

  
  virtual bool SetWindowTitle(const string16& title) = 0;

  
  
  
  virtual void SetWindowIcons(const gfx::ImageSkia& window_icon,
                              const gfx::ImageSkia& app_icon) = 0;

  
  
  
  virtual void InitModalType(ui::ModalType modal_type) = 0;

  
  virtual gfx::Rect GetWindowBoundsInScreen() const = 0;
  virtual gfx::Rect GetClientAreaBoundsInScreen() const = 0;
  virtual gfx::Rect GetRestoredBounds() const = 0;
  virtual void SetBounds(const gfx::Rect& bounds) = 0;
  virtual void SetSize(const gfx::Size& size) = 0;
  virtual void StackAbove(gfx::NativeView native_view) = 0;
  virtual void StackAtTop() = 0;
  virtual void StackBelow(gfx::NativeView native_view) = 0;
  virtual void SetShape(gfx::NativeRegion shape) = 0;
  virtual void Close() = 0;
  virtual void CloseNow() = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;
  
  
  virtual void ShowMaximizedWithBounds(const gfx::Rect& restored_bounds) = 0;
  virtual void ShowWithWindowState(ui::WindowShowState show_state) = 0;
  virtual bool IsVisible() const = 0;
  virtual void Activate() = 0;
  virtual void Deactivate() = 0;
  virtual bool IsActive() const = 0;
  virtual void SetAlwaysOnTop(bool always_on_top) = 0;
  virtual bool IsAlwaysOnTop() const = 0;
  virtual void Maximize() = 0;
  virtual void Minimize() = 0;
  virtual bool IsMaximized() const = 0;
  virtual bool IsMinimized() const = 0;
  virtual void Restore() = 0;
  virtual void SetFullscreen(bool fullscreen) = 0;
  virtual bool IsFullscreen() const = 0;
  virtual void SetOpacity(unsigned char opacity) = 0;
  virtual void SetUseDragFrame(bool use_drag_frame) = 0;
  virtual void FlashFrame(bool flash) = 0;
  virtual void RunShellDrag(View* view,
                            const ui::OSExchangeData& data,
                            const gfx::Point& location,
                            int operation,
                            ui::DragDropTypes::DragEventSource source) = 0;
  virtual void SchedulePaintInRect(const gfx::Rect& rect) = 0;
  virtual void SetCursor(gfx::NativeCursor cursor) = 0;
  virtual bool IsMouseEventsEnabled() const = 0;
  virtual void ClearNativeFocus() = 0;
  virtual gfx::Rect GetWorkAreaBoundsInScreen() const = 0;
  virtual Widget::MoveLoopResult RunMoveLoop(
      const gfx::Vector2d& drag_offset,
      Widget::MoveLoopSource source,
      Widget::MoveLoopEscapeBehavior escape_behavior) = 0;
  virtual void EndMoveLoop() = 0;
  virtual void SetVisibilityChangedAnimationsEnabled(bool value) = 0;
  virtual ui::NativeTheme* GetNativeTheme() const = 0;
  virtual void OnRootViewLayout() const = 0;

  
  virtual internal::NativeWidgetPrivate* AsNativeWidgetPrivate() OVERRIDE;
  virtual ui::EventHandler* GetEventHandler() = 0;
};

}  
}  

#endif  
