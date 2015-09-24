// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_APP_LIST_CONTROLLER_H_
#define ASH_WM_APP_LIST_CONTROLLER_H_

#include "ash/shelf/shelf_icon_observer.h"
#include "ash/shell_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/timer/timer.h"
#include "ui/app_list/pagination_model_observer.h"
#include "ui/aura/client/focus_change_observer.h"
#include "ui/aura/window_observer.h"
#include "ui/compositor/layer_animation_observer.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/rect.h"
#include "ui/views/widget/widget_observer.h"

namespace app_list {
class ApplicationDragAndDropHost;
class AppListView;
class PaginationModel;
}

namespace ui {
class LocatedEvent;
}

namespace ash {
namespace test {
class AppListControllerTestApi;
}

namespace internal {

class AppListController : public ui::EventHandler,
                          public aura::client::FocusChangeObserver,
                          public aura::WindowObserver,
                          public ui::ImplicitAnimationObserver,
                          public views::WidgetObserver,
                          public ShellObserver,
                          public ShelfIconObserver,
                          public app_list::PaginationModelObserver {
 public:
  AppListController();
  virtual ~AppListController();

  
  
  
  void SetVisible(bool visible, aura::Window* window);

  
  bool IsVisible() const;

  
  
  bool GetTargetVisibility() const { return is_visible_; }

  
  aura::Window* GetWindow();

  
  app_list::AppListView* GetView() { return view_; }

 private:
  friend class test::AppListControllerTestApi;

  
  
  void SetDragAndDropHostOfCurrentAppList(
      app_list::ApplicationDragAndDropHost* drag_and_drop_host);

  
  void SetView(app_list::AppListView* view);

  
  void ResetView();

  
  void ScheduleAnimation();

  void ProcessLocatedEvent(ui::LocatedEvent* event);

  
  void UpdateBounds();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void OnWindowFocused(aura::Window* gained_focus,
                               aura::Window* lost_focus) OVERRIDE;

  
  virtual void OnWindowBoundsChanged(aura::Window* root,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
  virtual void OnImplicitAnimationsCompleted() OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  virtual void OnShelfAlignmentChanged(aura::Window* root_window) OVERRIDE;

  
  virtual void OnShelfIconPositionsChanged() OVERRIDE;

  
  virtual void TotalPagesChanged() OVERRIDE;
  virtual void SelectedPageChanged(int old_selected, int new_selected) OVERRIDE;
  virtual void TransitionStarted() OVERRIDE;
  virtual void TransitionChanged() OVERRIDE;

  scoped_ptr<app_list::PaginationModel> pagination_model_;

  
  bool is_visible_;

  
  app_list::AppListView* view_;

  
  gfx::Rect view_bounds_;

  
  bool should_snap_back_;

  DISALLOW_COPY_AND_ASSIGN(AppListController);
};

}  
}  

#endif  
