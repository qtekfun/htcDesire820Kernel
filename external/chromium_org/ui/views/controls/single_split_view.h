// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SINGLE_SPLIT_VIEW_H_
#define UI_VIEWS_CONTROLS_SINGLE_SPLIT_VIEW_H_

#include "base/gtest_prod_util.h"
#include "ui/views/view.h"

namespace views {

class SingleSplitViewListener;

class VIEWS_EXPORT SingleSplitView : public View {
 public:
  enum Orientation {
    HORIZONTAL_SPLIT,
    VERTICAL_SPLIT
  };

  static const char kViewClassName[];

  SingleSplitView(View* leading,
                  View* trailing,
                  Orientation orientation,
                  SingleSplitViewListener* listener);

  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;

  Orientation orientation() const {
    return is_horizontal_ ? HORIZONTAL_SPLIT : VERTICAL_SPLIT;
  }

  void set_orientation(Orientation orientation) {
    is_horizontal_ = orientation == HORIZONTAL_SPLIT;
  }

  void set_divider_offset(int divider_offset) {
    divider_offset_ = divider_offset;
  }
  int divider_offset() const { return divider_offset_; }

  int GetDividerSize() const;

  void set_resize_disabled(bool resize_disabled) {
    resize_disabled_ = resize_disabled;
  }
  bool is_resize_disabled() const { return resize_disabled_; }

  
  
  
  void set_resize_leading_on_bounds_change(bool resize) {
    resize_leading_on_bounds_change_ = resize;
  }

  
  
  
  void CalculateChildrenBounds(const gfx::Rect& bounds,
                               gfx::Rect* leading_bounds,
                               gfx::Rect* trailing_bounds) const;

  void SetAccessibleName(const string16& name);

 protected:
  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  
  FRIEND_TEST_ALL_PREFIXES(SingleSplitViewTest, MouseDrag);

  
  bool IsPointInDivider(const gfx::Point& p);

  
  
  int CalculateDividerOffset(int divider_offset,
                             const gfx::Rect& previous_bounds,
                             const gfx::Rect& new_bounds) const;

  
  
  int NormalizeDividerOffset(int divider_offset, const gfx::Rect& bounds) const;

  
  int GetPrimaryAxisSize() const {
    return GetPrimaryAxisSize(width(), height());
  }

  int GetPrimaryAxisSize(int h, int v) const {
    return is_horizontal_ ? h : v;
  }

  
  struct DragInfo {
    
    int initial_mouse_offset;
    
    int initial_divider_offset;
  };

  DragInfo drag_info_;

  
  bool is_horizontal_;

  
  int divider_offset_;

  bool resize_leading_on_bounds_change_;

  
  bool resize_disabled_;

  
  SingleSplitViewListener* listener_;

  
  string16 accessible_name_;

  DISALLOW_COPY_AND_ASSIGN(SingleSplitView);
};

}  

#endif  
