// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_HOST_H_
#define UI_AURA_ROOT_WINDOW_HOST_H_

#include <vector>

#include "base/message_loop/message_loop.h"
#include "ui/aura/aura_export.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Insets;
class Point;
class Rect;
class Size;
}

namespace aura {

class RootWindow;
class RootWindowHostDelegate;

class AURA_EXPORT RootWindowHost {
 public:
  virtual ~RootWindowHost();

  
  static RootWindowHost* Create(const gfx::Rect& bounds);

  
  
  static gfx::Size GetNativeScreenSize();

  void set_delegate(RootWindowHostDelegate* delegate) {
    delegate_ = delegate;
  }

  virtual RootWindow* GetRootWindow() = 0;

  
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual void ToggleFullScreen() = 0;

  
  virtual gfx::Rect GetBounds() const = 0;
  virtual void SetBounds(const gfx::Rect& bounds) = 0;

  
  
  virtual gfx::Insets GetInsets() const = 0;
  virtual void SetInsets(const gfx::Insets& insets) = 0;

  
  
  void ConvertPointToNativeScreen(gfx::Point* point) const;

  
  void ConvertPointFromNativeScreen(gfx::Point* point) const;

  
  virtual void SetCapture() = 0;

  
  virtual void ReleaseCapture() = 0;

  
  virtual void SetCursor(gfx::NativeCursor cursor) = 0;

  
  
  
  
  
  
  virtual bool QueryMouseLocation(gfx::Point* location_return) = 0;

  
  
  
  
  
  virtual bool ConfineCursorToRootWindow() = 0;
  virtual void UnConfineCursor() = 0;

  
  virtual void OnCursorVisibilityChanged(bool show) = 0;

  
  virtual void MoveCursorTo(const gfx::Point& location) = 0;

  
  virtual void PostNativeEvent(const base::NativeEvent& native_event) = 0;

  
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) = 0;

  
  virtual void PrepareForShutdown() = 0;

 protected:
  RootWindowHost();

  
  virtual gfx::Point GetLocationOnNativeScreen() const = 0;

  RootWindowHostDelegate* delegate_;

 private:
  DISALLOW_COPY_AND_ASSIGN(RootWindowHost);
};

}  

#endif  
