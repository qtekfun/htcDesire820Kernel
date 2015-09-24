// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_WINDOW_SELECTOR_PANELS_H_
#define ASH_WM_OVERVIEW_WINDOW_SELECTOR_PANELS_H_

#include "ash/wm/overview/window_selector_item.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "ui/gfx/rect.h"

namespace ash {

class ScopedTransformOverviewWindow;

class WindowSelectorPanels : public WindowSelectorItem {
 public:
  WindowSelectorPanels();
  virtual ~WindowSelectorPanels();

  
  
  void AddWindow(aura::Window* window);

  
  virtual aura::Window* GetRootWindow() OVERRIDE;
  virtual bool HasSelectableWindow(const aura::Window* window) OVERRIDE;
  virtual aura::Window* TargetedWindow(const aura::Window* target) OVERRIDE;
  virtual void RestoreWindowOnExit(aura::Window* window) OVERRIDE;
  virtual aura::Window* SelectionWindow() OVERRIDE;
  virtual void RemoveWindow(const aura::Window* window) OVERRIDE;
  virtual bool empty() const OVERRIDE;
  virtual void PrepareForOverview() OVERRIDE;
  virtual void SetItemBounds(aura::Window* root_window,
                             const gfx::Rect& target_bounds,
                             bool animate) OVERRIDE;

 private:
  typedef ScopedVector<ScopedTransformOverviewWindow> WindowList;
  WindowList transform_windows_;

  DISALLOW_COPY_AND_ASSIGN(WindowSelectorPanels);
};

}  

#endif  
