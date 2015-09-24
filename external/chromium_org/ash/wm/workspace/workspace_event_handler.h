// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_WORKSPACE_EVENT_HANDLER_H_
#define ASH_WM_WORKSPACE_WORKSPACE_EVENT_HANDLER_H_

#include "ash/wm/toplevel_window_event_handler.h"
#include "ash/wm/workspace/multi_window_resize_controller.h"

namespace aura {
class Window;
}

namespace ash {
namespace wm {
class WindowState;
}

namespace internal {

class WorkspaceEventHandlerTestHelper;

class WorkspaceEventHandler : public ToplevelWindowEventHandler {
 public:
  explicit WorkspaceEventHandler(aura::Window* owner);
  virtual ~WorkspaceEventHandler();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  friend class WorkspaceEventHandlerTestHelper;

  
  
  
  
  void HandleVerticalResizeDoubleClick(wm::WindowState* window_state,
                                       ui::MouseEvent* event);

  MultiWindowResizeController multi_window_resize_controller_;

  DISALLOW_COPY_AND_ASSIGN(WorkspaceEventHandler);
};

}  
}  

#endif  
