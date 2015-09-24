// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_HOST_X11_H_
#define UI_AURA_ROOT_WINDOW_HOST_X11_H_

#include <X11/Xlib.h>

#include <vector>

#undef RootWindow

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/env_observer.h"
#include "ui/aura/window_tree_host.h"
#include "ui/base/x/x11_util.h"
#include "ui/events/event_source.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/x/x11_atom_cache.h"

namespace ui {
class MouseEvent;
}

namespace aura {

namespace internal {
class TouchEventCalibrate;
}

class AURA_EXPORT RootWindowHostX11 : public RootWindowHost,
                                      public base::MessageLoop::Dispatcher,
                                      public ui::EventSource,
                                      public EnvObserver {
 public:
  explicit RootWindowHostX11(const gfx::Rect& bounds);
  virtual ~RootWindowHostX11();

  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  
  virtual RootWindow* GetRootWindow() OVERRIDE;
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
  virtual void SetCursor(gfx::NativeCursor cursor_type) OVERRIDE;
  virtual bool QueryMouseLocation(gfx::Point* location_return) OVERRIDE;
  virtual bool ConfineCursorToRootWindow() OVERRIDE;
  virtual void UnConfineCursor() OVERRIDE;
  virtual void OnCursorVisibilityChanged(bool show) OVERRIDE;
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;
  virtual void PostNativeEvent(const base::NativeEvent& event) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void PrepareForShutdown() OVERRIDE;

  
  virtual void OnWindowInitialized(Window* window) OVERRIDE;
  virtual void OnRootWindowInitialized(RootWindow* root_window) OVERRIDE;

  
  virtual ui::EventProcessor* GetEventProcessor() OVERRIDE;

 private:
  class MouseMoveFilter;

  bool DispatchEventForRootWindow(const base::NativeEvent& event);

  
  
  
  void DispatchXI2Event(const base::NativeEvent& event);

  
  
  
  bool IsWindowManagerPresent();

  
  
  void SetCursorInternal(gfx::NativeCursor cursor);

  
  
  void TranslateAndDispatchMouseEvent(ui::MouseEvent* event);

  
  void UpdateIsInternalDisplay();

  
  
  void SetCrOSTapPaused(bool state);

  
  XDisplay* xdisplay_;
  ::Window xwindow_;

  
  ::Window x_root_window_;

  
  gfx::NativeCursor current_cursor_;

  
  bool window_mapped_;

  
  gfx::Rect bounds_;

  
  gfx::Insets insets_;

  
  bool is_internal_display_;

  scoped_ptr<XID[]> pointer_barriers_;

  scoped_ptr<internal::TouchEventCalibrate> touch_calibrate_;

  scoped_ptr<MouseMoveFilter> mouse_move_filter_;

  ui::X11AtomCache atom_cache_;

  
  uint32 bezel_tracking_ids_;

  DISALLOW_COPY_AND_ASSIGN(RootWindowHostX11);
};

namespace test {

AURA_EXPORT void SetUseOverrideRedirectWindowByDefault(bool override_redirect);

}  
}  

#endif  
