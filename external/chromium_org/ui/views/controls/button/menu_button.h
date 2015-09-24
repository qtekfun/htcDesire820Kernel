// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_MENU_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_MENU_BUTTON_H_

#include <string>

#include "base/strings/string16.h"
#include "base/time/time.h"
#include "ui/gfx/font.h"
#include "ui/views/background.h"
#include "ui/views/controls/button/text_button.h"

namespace views {

class MenuButtonListener;

class VIEWS_EXPORT MenuButton : public TextButton {
 public:
  static const char kViewClassName[];

  
  static const int kMenuMarkerPaddingLeft;
  static const int kMenuMarkerPaddingRight;

  
  MenuButton(ButtonListener* listener,
             const string16& text,
             MenuButtonListener* menu_button_listener,
             bool show_menu_marker);
  virtual ~MenuButton();

  bool show_menu_marker() const { return show_menu_marker_; }
  void set_menu_marker(const gfx::ImageSkia* menu_marker) {
    menu_marker_ = menu_marker;
  }
  const gfx::ImageSkia* menu_marker() const { return menu_marker_; }

  const gfx::Point& menu_offset() const { return menu_offset_; }
  void set_menu_offset(int x, int y) { menu_offset_.SetPoint(x, y); }

  
  virtual bool Activate();

  
  virtual void PaintButton(gfx::Canvas* canvas, PaintButtonMode mode) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  
  void PaintMenuMarker(gfx::Canvas* canvas);

  
  bool menu_visible_;

  
  gfx::Point menu_offset_;

 private:
  friend class TextButtonBackground;

  
  
  int GetMaximumScreenXCoordinate();

  
  
  
  
  
  
  base::TimeTicks menu_closed_time_;

  
  MenuButtonListener* listener_;

  
  bool show_menu_marker_;

  
  
  const gfx::ImageSkia* menu_marker_;

  
  
  bool* destroyed_flag_;

  DISALLOW_COPY_AND_ASSIGN(MenuButton);
};

}  

#endif  
