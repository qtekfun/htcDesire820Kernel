// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_CYCLE_CONTROLLER_H_
#define ASH_WM_WINDOW_CYCLE_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace ui {
class EventHandler;
}

namespace ash {

class WindowCycleList;

class ASH_EXPORT WindowCycleController {
 public:
  enum Direction {
    FORWARD,
    BACKWARD
  };
  WindowCycleController();
  virtual ~WindowCycleController();

  
  
  static bool CanCycle();

  
  
  
  void HandleCycleWindow(Direction direction, bool is_alt_down);

  
  
  void HandleLinearCycleWindow();

  
  
  void AltKeyReleased();

  
  bool IsCycling() const { return windows_.get() != NULL; }

  
  const WindowCycleList* windows() const { return windows_.get(); }

 private:
  
  void StartCycling();

  
  void Step(Direction direction);

  
  void InstallEventFilter();

  
  void StopCycling();

  scoped_ptr<WindowCycleList> windows_;

  
  scoped_ptr<ui::EventHandler> event_handler_;

  DISALLOW_COPY_AND_ASSIGN(WindowCycleController);
};

}  

#endif  
