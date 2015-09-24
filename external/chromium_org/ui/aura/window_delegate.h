// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_DELEGATE_H_
#define UI_AURA_WINDOW_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ui/aura/aura_export.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Canvas;
class Path;
class Point;
class Rect;
class Size;
}

namespace ui {
class GestureEvent;
class KeyEvent;
class Layer;
class MouseEvent;
class Texture;
class TouchEvent;
}

namespace aura {

class AURA_EXPORT WindowDelegate : public ui::EventHandler {
 public:
  
  virtual gfx::Size GetMinimumSize() const = 0;

  
  virtual gfx::Size GetMaximumSize() const = 0;

  
  virtual void OnBoundsChanged(const gfx::Rect& old_bounds,
                               const gfx::Rect& new_bounds) = 0;

  
  
  virtual gfx::NativeCursor GetCursor(const gfx::Point& point) = 0;

  
  
  virtual int GetNonClientComponent(const gfx::Point& point) const = 0;

  
  
  virtual bool ShouldDescendIntoChildForEventHandling(
      Window* child,
      const gfx::Point& location) = 0;

  
  virtual bool CanFocus() = 0;

  
  virtual void OnCaptureLost() = 0;

  
  virtual void OnPaint(gfx::Canvas* canvas) = 0;

  
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) = 0;

  
  
  
  virtual void OnWindowDestroying() = 0;

  
  
  
  
  virtual void OnWindowDestroyed() = 0;

  
  
  
  virtual void OnWindowTargetVisibilityChanged(bool visible) = 0;

  
  
  
  
  virtual bool HasHitTestMask() const = 0;

  
  
  virtual void GetHitTestMask(gfx::Path* mask) const = 0;

  
  
  
  
  
  virtual void DidRecreateLayer(ui::Layer* old_layer, ui::Layer* new_layer) = 0;

 protected:
  virtual ~WindowDelegate() {}
};

}  

#endif  
