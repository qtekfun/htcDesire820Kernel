// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_WINDOW_SELECTOR_H_
#define ASH_WM_OVERVIEW_WINDOW_SELECTOR_H_

#include <set>
#include <vector>

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/timer/timer.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/window_observer.h"

namespace aura {
class RootWindow;
}

namespace ui {
class EventHandler;
}

namespace ash {

namespace internal {
class WindowSelectorTest;
}

class ScopedShowWindow;
class WindowOverview;
class WindowSelectorDelegate;
class WindowSelectorItem;

class ASH_EXPORT WindowSelector
    : public aura::WindowObserver,
      public aura::client::ActivationChangeObserver {
 public:
  enum Direction {
    FORWARD,
    BACKWARD
  };
  enum Mode {
    CYCLE,
    OVERVIEW
  };

  typedef std::vector<aura::Window*> WindowList;

  WindowSelector(const WindowList& windows,
                 Mode mode,
                 WindowSelectorDelegate* delegate);
  virtual ~WindowSelector();

  
  void Step(Direction direction);

  
  void SelectWindow();

  
  void SelectWindow(aura::Window* window);

  
  void CancelSelection();

  Mode mode() { return mode_; }

  
  virtual void OnWindowAdded(aura::Window* new_window) OVERRIDE;
  virtual void OnWindowBoundsChanged(aura::Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;
  virtual void OnAttemptToReactivateWindow(
      aura::Window* request_active,
      aura::Window* actual_active) OVERRIDE;

 private:
  friend class internal::WindowSelectorTest;

  
  void StartOverview();

  
  
  void ResetFocusRestoreWindow(bool focus);

  
  
  ScopedVector<WindowSelectorItem> windows_;

  
  std::set<aura::Window*> observed_windows_;

  
  Mode mode_;

  
  
  scoped_ptr<ui::EventHandler> event_handler_;

  
  
  scoped_ptr<ScopedShowWindow> showing_window_;

  bool timer_enabled_;
  base::DelayTimer<WindowSelector> start_overview_timer_;
  scoped_ptr<WindowOverview> window_overview_;

  
  base::Time cycle_start_time_;

  
  
  WindowSelectorDelegate* delegate_;

  
  size_t selected_window_;

  
  
  
  aura::Window* restore_focus_window_;

  
  
  bool ignore_activations_;

  DISALLOW_COPY_AND_ASSIGN(WindowSelector);
};

}  

#endif  
