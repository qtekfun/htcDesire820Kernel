// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_SOLO_WINDOW_TRACKER_H_
#define ASH_WM_SOLO_WINDOW_TRACKER_H_

#include <vector>

#include "ash/ash_export.h"
#include "ash/wm/dock/docked_window_layout_manager_observer.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/rect.h"

namespace aura {
class RootWindow;
class Window;
}

namespace ash {

class ASH_EXPORT SoloWindowTracker
    : public aura::WindowObserver,
      public internal::DockedWindowLayoutManagerObserver {
 public:
  explicit SoloWindowTracker(aura::RootWindow* root_window);
  virtual ~SoloWindowTracker();

  
  static void SetSoloHeaderEnabled(bool enabled);

  
  aura::Window* GetWindowWithSoloHeader();

 private:
  
  
  
  
  void UpdateSoloWindow(aura::Window* ignore_window);

  
  bool AnyVisibleWindowDocked() const;

  
  virtual void OnWindowAdded(aura::Window* new_window) OVERRIDE;
  virtual void OnWillRemoveWindow(aura::Window* window) OVERRIDE;
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;

  
  virtual void OnDockBoundsChanging(const gfx::Rect& new_bounds,
                                    Reason reason) OVERRIDE;

  
  std::vector<aura::Window*> containers_;

  
  gfx::Rect dock_bounds_;

  
  
  aura::Window* solo_window_;

  
  class SoloWindowObserver;
  scoped_ptr<SoloWindowObserver> solo_window_observer_;

  DISALLOW_COPY_AND_ASSIGN(SoloWindowTracker);
};

}  

#endif 
