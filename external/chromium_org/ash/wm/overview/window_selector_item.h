// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_WINDOW_SELECTOR_ITEM_H_
#define ASH_WM_OVERVIEW_WINDOW_SELECTOR_ITEM_H_

#include "base/compiler_specific.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}

namespace ash {

class WindowSelectorItem {
 public:
  WindowSelectorItem();
  virtual ~WindowSelectorItem();

  
  virtual aura::Window* GetRootWindow() = 0;

  
  
  virtual bool HasSelectableWindow(const aura::Window* window) = 0;

  
  
  virtual aura::Window* TargetedWindow(const aura::Window* target) = 0;

  
  
  virtual void RestoreWindowOnExit(aura::Window* window) = 0;

  
  virtual aura::Window* SelectionWindow() = 0;

  
  
  virtual void RemoveWindow(const aura::Window* window) = 0;

  
  
  virtual bool empty() const = 0;

  
  
  virtual void PrepareForOverview() = 0;

  
  
  void SetBounds(aura::Window* root_window,
                 const gfx::Rect& target_bounds);

  
  
  void RecomputeWindowTransforms();

  const gfx::Rect& bounds() { return bounds_; }
  const gfx::Rect& target_bounds() { return target_bounds_; }

 protected:
  
  
  virtual void SetItemBounds(aura::Window* root_window,
                             const gfx::Rect& target_bounds,
                             bool animate) = 0;

  
  void set_bounds(const gfx::Rect& bounds) { bounds_ = bounds; }

 private:
  
  aura::Window* root_window_;

  
  gfx::Rect target_bounds_;

  
  
  gfx::Rect bounds_;

  
  
  
  bool in_bounds_update_;

  DISALLOW_COPY_AND_ASSIGN(WindowSelectorItem);
};

}  

#endif  
