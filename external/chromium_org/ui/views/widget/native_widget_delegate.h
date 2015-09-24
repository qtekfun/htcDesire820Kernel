// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_DELEGATE_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_DELEGATE_H_

#include <vector>

#include "ui/events/event_constants.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class Path;
class Point;
class Size;
}

namespace ui {
class GestureEvent;
class KeyEvent;
class Layer;
class MouseEvent;
class TouchEvent;
class ScrollEvent;
}

namespace views {
class InputMethod;
class Widget;

namespace internal {

class VIEWS_EXPORT NativeWidgetDelegate {
 public:
  virtual ~NativeWidgetDelegate() {}

  
  virtual bool IsModal() const = 0;

  
  virtual bool IsDialogBox() const = 0;

  
  virtual bool CanActivate() const = 0;

  virtual bool IsInactiveRenderingDisabled() const = 0;
  virtual void EnableInactiveRendering() = 0;

  
  virtual void OnNativeWidgetActivationChanged(bool active) = 0;

  
  virtual void OnNativeFocus(gfx::NativeView focused_view) = 0;
  virtual void OnNativeBlur(gfx::NativeView focused_view) = 0;

  
  virtual void OnNativeWidgetVisibilityChanging(bool visible) = 0;

  
  virtual void OnNativeWidgetVisibilityChanged(bool visible) = 0;

  
  
  
  virtual void OnNativeWidgetCreated(bool desktop_widget) = 0;

  
  
  virtual void OnNativeWidgetDestroying() = 0;

  
  virtual void OnNativeWidgetDestroyed() = 0;

  
  virtual gfx::Size GetMinimumSize() = 0;

  
  virtual gfx::Size GetMaximumSize() = 0;

  
  virtual void OnNativeWidgetMove() = 0;

  
  virtual void OnNativeWidgetSizeChanged(const gfx::Size& new_size) = 0;

  
  virtual void OnNativeWidgetBeginUserBoundsChange() = 0;
  virtual void OnNativeWidgetEndUserBoundsChange() = 0;

  
  virtual bool HasFocusManager() const = 0;

  
  
  virtual bool OnNativeWidgetPaintAccelerated(
      const gfx::Rect& dirty_region) = 0;

  
  
  virtual void OnNativeWidgetPaint(gfx::Canvas* canvas) = 0;

  
  
  virtual int GetNonClientComponent(const gfx::Point& point) = 0;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) = 0;
  virtual void OnMouseEvent(ui::MouseEvent* event) = 0;
  virtual void OnMouseCaptureLost() = 0;

  virtual void OnTouchEvent(ui::TouchEvent* event) = 0;
  virtual void OnScrollEvent(ui::ScrollEvent* event) = 0;
  virtual void OnGestureEvent(ui::GestureEvent* event) = 0;

  
  virtual bool ExecuteCommand(int command_id) = 0;

  
  
  
  virtual InputMethod* GetInputMethodDirect() = 0;

  
  virtual const std::vector<ui::Layer*>& GetRootLayers() = 0;

  
  virtual bool HasHitTestMask() const = 0;

  
  virtual void GetHitTestMask(gfx::Path* mask) const = 0;

  
  virtual Widget* AsWidget() = 0;
  virtual const Widget* AsWidget() const = 0;
};

}  
}  

#endif  
