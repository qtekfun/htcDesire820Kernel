// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIN_HWND_MESSAGE_HANDLER_DELEGATE_H_
#define UI_VIEWS_WIN_HWND_MESSAGE_HANDLER_DELEGATE_H_

#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class Insets;
class Path;
class Point;
class Size;
}

namespace ui {
class Accelerator;
class KeyEvent;
class MouseEvent;
class TouchEvent;
}

namespace views {

class InputMethod;
class NativeWidgetWin;

class VIEWS_EXPORT HWNDMessageHandlerDelegate {
 public:
  virtual bool IsWidgetWindow() const = 0;

  
  
  virtual bool IsUsingCustomFrame() const = 0;

  virtual void SchedulePaint() = 0;
  virtual void EnableInactiveRendering() = 0;
  virtual bool IsInactiveRenderingDisabled() = 0;

  virtual bool CanResize() const = 0;
  virtual bool CanMaximize() const = 0;
  virtual bool CanActivate() const = 0;

  virtual bool WidgetSizeIsClientSize() const = 0;

  
  
  virtual bool CanSaveFocus() const = 0;
  virtual void SaveFocusOnDeactivate() = 0;
  virtual void RestoreFocusOnActivate() = 0;
  virtual void RestoreFocusOnEnable() = 0;

  
  virtual bool IsModal() const = 0;

  
  
  virtual int GetInitialShowState() const = 0;

  virtual bool WillProcessWorkAreaChange() const = 0;

  virtual int GetNonClientComponent(const gfx::Point& point) const = 0;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* mask) = 0;

  
  
  
  virtual bool GetClientAreaInsets(gfx::Insets* insets) const = 0;

  
  
  virtual void GetMinMaxSize(gfx::Size* min_size,
                             gfx::Size* max_size) const = 0;

  
  virtual gfx::Size GetRootViewSize() const = 0;

  virtual void ResetWindowControls() = 0;

  virtual void PaintLayeredWindow(gfx::Canvas* canvas) = 0;

  virtual InputMethod* GetInputMethod() = 0;

  virtual gfx::NativeViewAccessible GetNativeViewAccessible() = 0;

  
  
  
  
  virtual bool ShouldHandleSystemCommands() const = 0;

  
  

  
  virtual void HandleAppDeactivated() = 0;

  
  
  virtual void HandleActivationChanged(bool active) = 0;

  
  
  virtual bool HandleAppCommand(short command) = 0;

  
  virtual void HandleCancelMode() = 0;

  
  virtual void HandleCaptureLost() = 0;

  
  virtual void HandleClose() = 0;

  
  
  virtual bool HandleCommand(int command) = 0;

  
  virtual void HandleAccelerator(const ui::Accelerator& accelerator) = 0;

  
  virtual void HandleCreate() = 0;

  
  
  virtual void HandleDestroying() = 0;

  
  
  virtual void HandleDestroyed() = 0;

  
  
  
  virtual bool HandleInitialFocus() = 0;

  
  virtual void HandleDisplayChange() = 0;

  
  
  virtual void HandleBeginWMSizeMove() = 0;
  virtual void HandleEndWMSizeMove() = 0;

  
  virtual void HandleMove() = 0;

  
  virtual void HandleWorkAreaChanged() = 0;

  
  
  virtual void HandleVisibilityChanging(bool visible) = 0;

  
  virtual void HandleVisibilityChanged(bool visible) = 0;

  
  
  virtual void HandleClientSizeChanged(const gfx::Size& new_size) = 0;

  
  virtual void HandleFrameChanged() = 0;

  
  virtual void HandleNativeFocus(HWND last_focused_window) = 0;

  
  virtual void HandleNativeBlur(HWND focused_window) = 0;

  
  
  virtual bool HandleMouseEvent(const ui::MouseEvent& event) = 0;

  
  
  virtual bool HandleKeyEvent(const ui::KeyEvent& event) = 0;

  
  
  virtual bool HandleUntranslatedKeyEvent(const ui::KeyEvent& event) = 0;

  
  virtual void HandleTouchEvent(const ui::TouchEvent& event) = 0;

  
  
  
  virtual bool HandleIMEMessage(UINT message,
                                WPARAM w_param,
                                LPARAM l_param,
                                LRESULT* result) = 0;

  
  virtual void HandleInputLanguageChange(DWORD character_set,
                                         HKL input_language_id) = 0;

  
  
  virtual bool HandlePaintAccelerated(const gfx::Rect& invalid_rect) = 0;

  
  virtual void HandlePaint(gfx::Canvas* canvas) = 0;

  
  virtual bool HandleTooltipNotify(int w_param,
                                   NMHDR* l_param,
                                   LRESULT* l_result) = 0;

  
  virtual void HandleTooltipMouseMove(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) = 0;

  
  
  
  
  
  
  virtual bool PreHandleMSG(UINT message,
                            WPARAM w_param,
                            LPARAM l_param,
                            LRESULT* result) = 0;

  
  
  virtual void PostHandleMSG(UINT message,
                             WPARAM w_param,
                             LPARAM l_param) = 0;

  
  
  virtual bool HandleScrollEvent(const ui::ScrollEvent& event) = 0;

 protected:
  virtual ~HWNDMessageHandlerDelegate() {}
};

}  

#endif  
