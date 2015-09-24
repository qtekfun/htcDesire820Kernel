// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_PANELS_PANEL_WINDOW_EVENT_HANDLER_H_
#define ASH_WM_PANELS_PANEL_WINDOW_EVENT_HANDLER_H_

#include "ash/wm/toplevel_window_event_handler.h"

namespace aura {
class Window;
}

namespace ash {
namespace internal {

class PanelWindowEventHandler : public ToplevelWindowEventHandler {
 public:
  explicit PanelWindowEventHandler(aura::Window* owner);
  virtual ~PanelWindowEventHandler();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(PanelWindowEventHandler);
};

}  
}  

#endif  
