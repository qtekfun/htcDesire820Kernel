// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TOUCHUI_TOUCH_EDITING_MENU_H_
#define UI_VIEWS_TOUCHUI_TOUCH_EDITING_MENU_H_

#include "ui/gfx/point.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
}

namespace views {
class TouchEditingMenuView;
class Widget;

class VIEWS_EXPORT TouchEditingMenuController {
 public:
  
  virtual bool IsCommandIdEnabled(int command_id) const = 0;

  
  virtual void ExecuteCommand(int command_id, int event_flags) = 0;

  
  virtual void OpenContextMenu() = 0;

  
  virtual void OnMenuClosed(TouchEditingMenuView* menu) = 0;

 protected:
  virtual ~TouchEditingMenuController() {}
};

class VIEWS_EXPORT TouchEditingMenuView : public BubbleDelegateView,
                                          public ButtonListener {
 public:
  virtual ~TouchEditingMenuView();

  
  
  static TouchEditingMenuView* Create(TouchEditingMenuController* controller,
                                      gfx::Rect anchor_rect,
                                      gfx::NativeView context);

  void Close();

 private:
  TouchEditingMenuView(TouchEditingMenuController* controller,
                       gfx::Rect anchor_rect,
                       gfx::NativeView context);

  
  virtual void WindowClosing() OVERRIDE;

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  
  void CreateButtons();

  
  Button* CreateButton(const string16& title, int tag);

  TouchEditingMenuController* controller_;

  DISALLOW_COPY_AND_ASSIGN(TouchEditingMenuView);
};

}  

#endif  
