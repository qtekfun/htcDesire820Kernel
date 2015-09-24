// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_OVERFLOW_BUBBLE_VIEW_H_
#define ASH_SHELF_OVERFLOW_BUBBLE_VIEW_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/bubble/bubble_delegate.h"

namespace ash {

namespace test {
class OverflowBubbleViewTestAPI;
}

namespace internal {

class ShelfLayoutManager;
class ShelfView;

class ASH_EXPORT OverflowBubbleView : public views::BubbleDelegateView {
 public:
  OverflowBubbleView();
  virtual ~OverflowBubbleView();

  void InitOverflowBubble(views::View* anchor, ShelfView* shelf_view);

  
  virtual gfx::Rect GetBubbleBounds() OVERRIDE;

 private:
  friend class test::OverflowBubbleViewTestAPI;

  bool IsHorizontalAlignment() const;

  const gfx::Size GetContentsSize() const;

  
  views::BubbleBorder::Arrow GetBubbleArrow() const;

  void ScrollByXOffset(int x_offset);
  void ScrollByYOffset(int y_offset);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;

  ShelfLayoutManager* GetShelfLayoutManagerForLauncher() const;

  ShelfView* shelf_view_;  
  gfx::Vector2d scroll_offset_;

  DISALLOW_COPY_AND_ASSIGN(OverflowBubbleView);
};

}  
}  

#endif  
