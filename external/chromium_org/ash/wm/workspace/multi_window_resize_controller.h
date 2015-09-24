// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_MULTI_WINDOW_RESIZE_CONTROLLER_H_
#define ASH_WM_WORKSPACE_MULTI_WINDOW_RESIZE_CONTROLLER_H_

#include <vector>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/rect.h"
#include "ui/views/mouse_watcher.h"

namespace aura {
class Window;
}

namespace views {
class Widget;
}

namespace ash {
namespace internal {

class MultiWindowResizeControllerTest;
class WorkspaceWindowResizer;

enum Direction {
  TOP_BOTTOM,
  LEFT_RIGHT,
};

class ASH_EXPORT MultiWindowResizeController :
    public views::MouseWatcherListener, public aura::WindowObserver {
 public:
  MultiWindowResizeController();
  virtual ~MultiWindowResizeController();

  
  
  void Show(aura::Window* window, int component, const gfx::Point& point);

  
  void Hide();

  
  virtual void MouseMovedOutOfHost() OVERRIDE;

  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

 private:
  friend class MultiWindowResizeControllerTest;

  
  struct ResizeWindows {
    ResizeWindows();
    ~ResizeWindows();

    
    
    bool Equals(const ResizeWindows& other) const;

    
    bool is_valid() const { return window1 && window2; }

    
    aura::Window* window1;

    
    aura::Window* window2;

    
    Direction direction;

    
    
    std::vector<aura::Window*> other_windows;
  };

  class ResizeMouseWatcherHost;
  class ResizeView;

  
  
  ResizeWindows DetermineWindows(aura::Window* window,
                                 int window_component,
                                 const gfx::Point& point) const;

  
  
  ResizeWindows DetermineWindowsFromScreenPoint(aura::Window* window) const;

  
  aura::Window* FindWindowByEdge(aura::Window* window_to_ignore,
                                 int edge_want,
                                 int x,
                                 int y) const;

  
  aura::Window* FindWindowTouching(aura::Window* window,
                                   Direction direction) const;

  
  void FindWindowsTouching(aura::Window* start,
                           Direction direction,
                           std::vector<aura::Window*>* others) const;

  
  void DelayedHide();

  
  
  void ShowIfValidMouseLocation();

  
  void ShowNow();

  
  bool IsShowing() const;

  
  void StartResize(const gfx::Point& location_in_screen);

  
  void Resize(const gfx::Point& location_in_screen, int event_flags);

  
  void CompleteResize(int event_flags);

  
  void CancelResize();

  
  gfx::Rect CalculateResizeWidgetBounds(
      const gfx::Point& location_in_parent) const;

  
  
  bool IsOverWindows(const gfx::Point& location_in_screen) const;

  
  bool IsOverWindow(aura::Window* window,
                    const gfx::Point& location_in_screen,
                    int component) const;

  
  ResizeWindows windows_;

  
  base::OneShotTimer<MultiWindowResizeController> hide_timer_;

  
  base::OneShotTimer<MultiWindowResizeController> show_timer_;

  scoped_ptr<views::Widget> resize_widget_;

  
  scoped_ptr<WorkspaceWindowResizer> window_resizer_;

  
  gfx::Point show_location_in_parent_;

  
  gfx::Rect show_bounds_in_screen_;

  
  
  
  scoped_ptr<views::MouseWatcher> mouse_watcher_;

  DISALLOW_COPY_AND_ASSIGN(MultiWindowResizeController);
};

}  
}  

#endif  
