// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_BASE_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_BASE_SCROLL_BAR_H_

#include "base/gtest_prod_util.h"
#include "ui/views/animation/scroll_animator.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/menu/menu_delegate.h"
#include "ui/views/controls/scrollbar/scroll_bar.h"
#include "ui/views/repeat_controller.h"

namespace views {

class BaseScrollBarThumb;
class MenuRunner;

class VIEWS_EXPORT BaseScrollBar : public ScrollBar,
                                   public ScrollDelegate,
                                   public ContextMenuController,
                                   public MenuDelegate {
 public:
  BaseScrollBar(bool horizontal, BaseScrollBarThumb* thumb);
  virtual ~BaseScrollBar();

  
  virtual gfx::Rect GetTrackBounds() const = 0;

  
  
  enum ScrollAmount {
    SCROLL_NONE = 0,
    SCROLL_START,
    SCROLL_END,
    SCROLL_PREV_LINE,
    SCROLL_NEXT_LINE,
    SCROLL_PREV_PAGE,
    SCROLL_NEXT_PAGE,
  };

  
  void ScrollByAmount(ScrollAmount amount);

  
  
  
  
  void ScrollToThumbPosition(int thumb_position, bool scroll_to_middle);

  
  bool ScrollByContentsOffset(int contents_offset);

  
  
  void OnThumbStateChanged(CustomButton::ButtonState old_state,
                           CustomButton::ButtonState new_state);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE = 0;
  virtual void Layout() OVERRIDE = 0;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void Update(int viewport_size,
                      int content_size,
                      int contents_scroll_offset) OVERRIDE;
  virtual int GetLayoutSize() const OVERRIDE = 0;
  virtual int GetPosition() const OVERRIDE;

  
  virtual bool OnScroll(float dx, float dy) OVERRIDE;

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual string16 GetLabel(int id) const OVERRIDE;
  virtual bool IsCommandEnabled(int id) const OVERRIDE;
  virtual void ExecuteCommand(int id) OVERRIDE;

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE = 0;

  BaseScrollBarThumb* GetThumb() const;
  CustomButton::ButtonState GetThumbTrackState() const;

  
  
  
  
  
  virtual void ScrollToPosition(int position);
  virtual int GetScrollIncrement(bool is_page, bool is_positive);

 private:
  FRIEND_TEST_ALL_PREFIXES(NativeScrollBarTest, ScrollBarFitsToBottom);
  int GetThumbSizeForTest();

  
  void ProcessPressEvent(const ui::LocatedEvent& event);

  
  void SetState(CustomButton::ButtonState state);

  
  void TrackClicked();

  
  
  void ScrollContentsToOffset();

  
  int GetTrackSize() const;

  
  
  int CalculateThumbPosition(int contents_scroll_offset) const;

  
  
  
  int CalculateContentsOffset(int thumb_position,
                              bool scroll_to_middle) const;

  
  
  void SetThumbTrackState(CustomButton::ButtonState state);

  BaseScrollBarThumb* thumb_;

  
  int contents_size_;

  
  int contents_scroll_offset_;

  
  int viewport_size_;

  
  
  CustomButton::ButtonState thumb_track_state_;

  
  
  
  ScrollAmount last_scroll_amount_;

  
  
  
  RepeatController repeater_;

  
  
  int context_menu_mouse_position_;

  scoped_ptr<MenuRunner> menu_runner_;
  scoped_ptr<ScrollAnimator> scroll_animator_;

  DISALLOW_COPY_AND_ASSIGN(BaseScrollBar);
};

}  

#endif  
