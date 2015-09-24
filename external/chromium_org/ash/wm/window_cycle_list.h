// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_CYCLE_LIST_H_
#define ASH_WM_WINDOW_CYCLE_LIST_H_

#include <vector>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/window_observer.h"

namespace ash {

class ASH_EXPORT WindowCycleList : public aura::WindowObserver {
 public:
  typedef std::vector<aura::Window*> WindowList;

  enum Direction {
    FORWARD,
    BACKWARD
  };

  explicit WindowCycleList(const WindowList& windows);
  virtual ~WindowCycleList();

  bool empty() const { return windows_.empty(); }

  
  void Step(Direction direction);

  const WindowList& windows() const { return windows_; }

 private:
  
  int GetWindowIndex(aura::Window* window);

  
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

  
  
  
  
  WindowList windows_;

  
  int current_index_;

  DISALLOW_COPY_AND_ASSIGN(WindowCycleList);
};

}  

#endif  
