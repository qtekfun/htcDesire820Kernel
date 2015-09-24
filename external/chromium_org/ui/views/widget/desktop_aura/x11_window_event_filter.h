// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WINDOW_EVENT_FILTER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WINDOW_EVENT_FILTER_H_

#include <X11/Xlib.h>
#undef RootWindow

#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/x/x11_atom_cache.h"
#include "ui/gfx/x/x11_types.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
class Window;
}

namespace gfx {
class Point;
}

namespace views {
class DesktopRootWindowHost;
class NativeWidgetAura;

class VIEWS_EXPORT X11WindowEventFilter : public ui::EventHandler {
 public:
  X11WindowEventFilter(aura::RootWindow* root_window,
                       DesktopRootWindowHost* root_window_host);
  virtual ~X11WindowEventFilter();

  
  void SetUseHostWindowBorders(bool use_os_border);

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;

 private:
  
  
  bool DispatchHostWindowDragMovement(int hittest,
                                      const gfx::Point& screen_location);

  
  XDisplay* xdisplay_;
  ::Window xwindow_;

  
  ::Window x_root_window_;

  ui::X11AtomCache atom_cache_;

  DesktopRootWindowHost* root_window_host_;

  
  bool is_active_;

  DISALLOW_COPY_AND_ASSIGN(X11WindowEventFilter);
};

}  

#endif  
