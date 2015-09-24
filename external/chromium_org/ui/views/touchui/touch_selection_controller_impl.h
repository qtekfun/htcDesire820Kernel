// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_UI_VIEWS_TOUCHUI_TOUCH_SELECTION_CONTROLLER_IMPL_H_
#define UI_UI_VIEWS_TOUCHUI_TOUCH_SELECTION_CONTROLLER_IMPL_H_

#include "base/timer/timer.h"
#include "ui/base/touch/touch_editing_controller.h"
#include "ui/gfx/point.h"
#include "ui/views/touchui/touch_editing_menu.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT TouchSelectionControllerImpl
    : public ui::TouchSelectionController,
      public TouchEditingMenuController,
      public WidgetObserver {
 public:
  
  explicit TouchSelectionControllerImpl(
      ui::TouchEditable* client_view);

  virtual ~TouchSelectionControllerImpl();

  
  virtual void SelectionChanged() OVERRIDE;
  virtual bool IsHandleDragInProgress() OVERRIDE;

 private:
  friend class TouchSelectionControllerImplTest;
  class EditingHandleView;

  void SetDraggingHandle(EditingHandleView* handle);

  
  
  void SelectionHandleDragged(const gfx::Point& drag_pos);

  
  
  void ConvertPointToClientView(EditingHandleView* source, gfx::Point* point);

  
  
  void SetHandleSelectionRect(EditingHandleView* handle, const gfx::Rect& rect,
                              const gfx::Rect& rect_in_screen);

  
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual void OpenContextMenu() OVERRIDE;
  virtual void OnMenuClosed(TouchEditingMenuView* menu) OVERRIDE;

  
  
  
  virtual void OnWidgetClosing(Widget* widget) OVERRIDE;
  virtual void OnWidgetBoundsChanged(Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
  void ContextMenuTimerFired();

  void StartContextMenuTimer();

  
  void UpdateContextMenu(const gfx::Point& p1, const gfx::Point& p2);

  
  void HideContextMenu();

  
  gfx::Point GetSelectionHandle1Position();
  gfx::Point GetSelectionHandle2Position();
  gfx::Point GetCursorHandlePosition();
  bool IsSelectionHandle1Visible();
  bool IsSelectionHandle2Visible();
  bool IsCursorHandleVisible();

  ui::TouchEditable* client_view_;
  Widget* client_widget_;
  scoped_ptr<EditingHandleView> selection_handle_1_;
  scoped_ptr<EditingHandleView> selection_handle_2_;
  scoped_ptr<EditingHandleView> cursor_handle_;
  TouchEditingMenuView* context_menu_;

  
  
  
  base::OneShotTimer<TouchSelectionControllerImpl> context_menu_timer_;

  
  EditingHandleView* dragging_handle_;

  
  
  
  
  
  gfx::Rect selection_end_point_1_;
  gfx::Rect selection_end_point_2_;

  DISALLOW_COPY_AND_ASSIGN(TouchSelectionControllerImpl);
};

class VIEWS_EXPORT ViewsTouchSelectionControllerFactory
    : public ui::TouchSelectionControllerFactory {
 public:
  ViewsTouchSelectionControllerFactory();

  
  virtual ui::TouchSelectionController* create(
      ui::TouchEditable* client_view) OVERRIDE;
};

}  

#endif  
