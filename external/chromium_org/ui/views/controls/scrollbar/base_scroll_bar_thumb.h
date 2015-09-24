// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_BASE_SCROLL_BAR_THUMB_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_BASE_SCROLL_BAR_THUMB_H_

#include "ui/gfx/size.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/controls/scrollbar/scroll_bar.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
}

namespace views {

class BaseScrollBar;

class VIEWS_EXPORT BaseScrollBarThumb : public View {
 public:
  explicit BaseScrollBarThumb(BaseScrollBar* scroll_bar);
  virtual ~BaseScrollBarThumb();

  
  void SetSize(int size);

  
  int GetSize() const;

  
  void SetPosition(int position);

  
  int GetPosition() const;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE = 0;

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE = 0;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;

  CustomButton::ButtonState GetState() const;
  
  void SetState(CustomButton::ButtonState state);

  BaseScrollBar* scroll_bar() { return scroll_bar_; }

 private:
  
  BaseScrollBar* scroll_bar_;

  int drag_start_position_;

  
  
  int mouse_offset_;

  
  CustomButton::ButtonState state_;

  DISALLOW_COPY_AND_ASSIGN(BaseScrollBarThumb);
};

}  

#endif  
