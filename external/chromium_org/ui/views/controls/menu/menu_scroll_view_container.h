// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_SCROLL_VIEW_CONTAINER_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_SCROLL_VIEW_CONTAINER_H_

#include "ui/views/view.h"
#include "ui/views/bubble/bubble_border.h"
#include "ui/views/controls/menu/menu_item_view.h"

namespace views {

class SubmenuView;

class MenuScrollViewContainer : public View {
 public:
  explicit MenuScrollViewContainer(SubmenuView* content_view);

  
  View* scroll_down_button() const { return scroll_down_button_; }
  View* scroll_up_button() const { return scroll_up_button_; }

  
  bool HasBubbleBorder();

  
  void SetBubbleArrowOffset(int offset);

  
  virtual void OnPaintBackground(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  
  void CreateDefaultBorder();

  
  void CreateBubbleBorder();

  BubbleBorder::Arrow BubbleBorderTypeFromAnchor(
      MenuItemView::AnchorPosition anchor);

  class MenuScrollView;

  
  View* scroll_up_button_;
  View* scroll_down_button_;

  
  MenuScrollView* scroll_view_;

  
  SubmenuView* content_view_;

  
  BubbleBorder::Arrow arrow_;

  
  BubbleBorder* bubble_border_;

  DISALLOW_COPY_AND_ASSIGN(MenuScrollViewContainer);
};

}  

#endif  
