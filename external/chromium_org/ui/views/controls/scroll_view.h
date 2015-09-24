// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLL_VIEW_H_
#define UI_VIEWS_CONTROLS_SCROLL_VIEW_H_

#include <string>

#include "base/compiler_specific.h"
#include "ui/views/controls/scrollbar/scroll_bar.h"

namespace views {


class VIEWS_EXPORT ScrollView : public View, public ScrollBarController {
 public:
  static const char kViewClassName[];

  ScrollView();
  virtual ~ScrollView();

  
  static ScrollView* CreateScrollViewWithBorder();

  
  
  void SetContents(View* a_view);
  const View* contents() const { return contents_; }
  View* contents() { return contents_; }

  
  void SetHeader(View* header);

  
  gfx::Rect GetVisibleRect() const;

  void set_hide_horizontal_scrollbar(bool visible) {
    hide_horizontal_scrollbar_ = visible;
  }

  
  
  int GetScrollBarWidth() const;
  int GetScrollBarHeight() const;

  
  const ScrollBar* horizontal_scroll_bar() const { return horiz_sb_; }
  const ScrollBar* vertical_scroll_bar() const { return vert_sb_; }

  
  
  void SetHorizontalScrollBar(ScrollBar* horiz_sb);
  void SetVerticalScrollBar(ScrollBar* vert_sb);

  
  virtual void Layout() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& e) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual void ScrollToPosition(ScrollBar* source, int position) OVERRIDE;
  virtual int GetScrollIncrement(ScrollBar* source,
                                 bool is_page,
                                 bool is_positive) OVERRIDE;

 private:
  class Viewport;

  
  
  void SetHeaderOrContents(View* parent, View* new_view, View** member);

  
  
  
  
  void ScrollContentsRegionToBeVisible(const gfx::Rect& rect);

  
  
  void ComputeScrollBarsVisibility(const gfx::Size& viewport_size,
                                   const gfx::Size& content_size,
                                   bool* horiz_is_shown,
                                   bool* vert_is_shown) const;

  
  
  void SetControlVisibility(View* control, bool should_show);

  
  void UpdateScrollBarPositions();

  
  
  View* contents_;
  View* contents_viewport_;

  
  
  View* header_;
  View* header_viewport_;

  
  ScrollBar* horiz_sb_;

  
  ScrollBar* vert_sb_;

  
  View* resize_corner_;

  
  
  bool hide_horizontal_scrollbar_;

  DISALLOW_COPY_AND_ASSIGN(ScrollView);
};

class VariableRowHeightScrollHelper {
 public:
  
  struct RowInfo {
    RowInfo(int origin, int height) : origin(origin), height(height) {}

    
    int origin;

    
    int height;
  };

  
  class Controller {
   public:
    
    virtual VariableRowHeightScrollHelper::RowInfo GetRowInfo(int y) = 0;
  };

  
  
  explicit VariableRowHeightScrollHelper(Controller* controller);
  virtual ~VariableRowHeightScrollHelper();

  
  
  
  int GetPageScrollIncrement(ScrollView* scroll_view,
                             bool is_horizontal, bool is_positive);
  int GetLineScrollIncrement(ScrollView* scroll_view,
                             bool is_horizontal, bool is_positive);

 protected:
  
  
  virtual RowInfo GetRowInfo(int y);

 private:
  Controller* controller_;

  DISALLOW_COPY_AND_ASSIGN(VariableRowHeightScrollHelper);
};

class FixedRowHeightScrollHelper : public VariableRowHeightScrollHelper {
 public:
  
  
  
  FixedRowHeightScrollHelper(int top_margin, int row_height);

 protected:
  
  virtual RowInfo GetRowInfo(int y) OVERRIDE;

 private:
  int top_margin_;
  int row_height_;

  DISALLOW_COPY_AND_ASSIGN(FixedRowHeightScrollHelper);
};

}  

#endif  
