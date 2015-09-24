// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_WINDOW_OVERVIEW_H_
#define ASH_WM_OVERVIEW_WINDOW_OVERVIEW_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/time/time.h"
#include "ui/aura/window_tracker.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
namespace client {
class CursorClient;
}
}  

namespace ui {
class LocatedEvent;
}

namespace views {
class Widget;
}

namespace ash {

class WindowSelector;
class WindowSelectorItem;

class WindowOverview : public ui::EventHandler {
 public:
  typedef ScopedVector<WindowSelectorItem> WindowSelectorItemList;

  
  
  
  
  WindowOverview(WindowSelector* window_selector,
                 WindowSelectorItemList* windows,
                 aura::Window* single_root_window);
  virtual ~WindowOverview();

  
  void SetSelection(size_t index);

  
  void OnWindowsChanged();

  
  void MoveToSingleRootWindow(aura::Window* root_window);

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;

 private:
  
  
  aura::Window* GetEventTarget(ui::LocatedEvent* event);

  
  
  aura::Window* GetTargetedWindow(aura::Window* window);

  
  void HideAndTrackNonOverviewWindows();

  
  void PositionWindows();
  
  void PositionWindowsFromRoot(aura::Window* root_window);
  
  void PositionWindowsOnRoot(aura::Window* root_window,
                             const std::vector<WindowSelectorItem*>& windows);

  
  void InitializeSelectionWidget();

  
  gfx::Rect GetSelectionBounds(size_t index);

  
  WindowSelector* window_selector_;

  
  
  
  WindowSelectorItemList* windows_;

  
  scoped_ptr<views::Widget> selection_widget_;

  
  
  size_t selection_index_;

  
  
  
  aura::Window* single_root_window_;

  
  base::Time overview_start_time_;

  
  aura::client::CursorClient* cursor_client_;

  
  
  aura::WindowTracker hidden_windows_;

  DISALLOW_COPY_AND_ASSIGN(WindowOverview);
};

}  

#endif  
