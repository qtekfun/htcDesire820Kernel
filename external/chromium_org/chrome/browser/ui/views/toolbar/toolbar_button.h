// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_BUTTON_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_BUTTON_H_

#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"

namespace ui {
class MenuModel;
}

namespace views {
class MenuRunner;
}

class ToolbarButton : public views::LabelButton,
                      public views::ContextMenuController {
 public:
  
  
  ToolbarButton(views::ButtonListener* listener, ui::MenuModel* model);
  virtual ~ToolbarButton();

  
  
  void Init();

  
  void ClearPendingMenu();
  bool IsMenuShowing() const;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

 protected:
  
  
  
  
  virtual bool ShouldEnterPushedState(const ui::Event& event) OVERRIDE;

  
  virtual bool ShouldShowMenu();

  
  virtual void ShowDropDownMenu(ui::MenuSourceType source_type);

 private:
  
  scoped_ptr<ui::MenuModel> model_;

  
  bool menu_showing_;

  
  int y_position_on_lbuttondown_;

  
  scoped_ptr<views::MenuRunner> menu_runner_;

  
  base::WeakPtrFactory<ToolbarButton> show_menu_factory_;

  DISALLOW_COPY_AND_ASSIGN(ToolbarButton);
};

#endif  
