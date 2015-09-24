// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_HOST_ROOT_VIEW_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_HOST_ROOT_VIEW_H_

#include "ui/views/widget/root_view.h"

namespace views {

class MenuController;
class SubmenuView;

class MenuHostRootView : public internal::RootView {
 public:
  MenuHostRootView(Widget* widget, SubmenuView* submenu);

  void ClearSubmenu() { submenu_ = NULL; }

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  
  virtual void DispatchGestureEvent(ui::GestureEvent* e) OVERRIDE;

 private:
  
  MenuController* GetMenuController();

  
  bool DoesEventTargetEmptyMenuItem(const ui::MouseEvent& event);

  
  SubmenuView* submenu_;

  
  bool forward_drag_to_menu_controller_;

  DISALLOW_COPY_AND_ASSIGN(MenuHostRootView);
};

}  

#endif  
