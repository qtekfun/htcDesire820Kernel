// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_STACKED_TAB_STRIP_LAYOUT_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_STACKED_TAB_STRIP_LAYOUT_H_

#include <algorithm>

#include "base/basictypes.h"
#include "ui/gfx/size.h"
#include "ui/views/view_model.h"

namespace views {
class ViewModel;
}

class StackedTabStripLayout {
 public:
  static const int kAddTypeMini   = 1 << 0;
  static const int kAddTypeActive = 1 << 1;

  
  
  
  
  
  StackedTabStripLayout(const gfx::Size& size,
                        int padding,
                        int stacked_padding,
                        int max_stacked_count,
                        views::ViewModel* view_model);
  ~StackedTabStripLayout();

  
  
  void SetXAndMiniCount(int x, int mini_tab_count);

  
  void SetWidth(int width);

  int width() const { return width_; }

  
  void SetActiveIndex(int index);

  
  void DragActiveTab(int delta);

  
  
  void SizeToFit();

  
  
  void AddTab(int index, int add_types, int start_x);

  
  
  
  void RemoveTab(int index, int start_x, int old_x);

  
  
  void MoveTab(int from,
               int to,
               int new_active_index,
               int start_x,
               int mini_tab_count);

  
  
  
  int active_index() const {
    return active_index_ < mini_tab_count_ ? mini_tab_count_ : active_index_;
  }

  int mini_tab_count() const { return mini_tab_count_; }

  
  bool IsStacked(int index) const;

  
  void SetActiveTabLocation(int x);

#if !defined(NDEBUG)
  std::string BoundsString() const;
#endif

 private:
  friend class StackedTabStripLayoutTest;

  
  
  void Reset(int x, int width, int mini_tab_count, int active_index);

  
  void ResetToIdealState();

  
  
  void MakeVisible(int index);

  
  
  int ConstrainActiveX(int x) const;

  
  
  void SetActiveBoundsAndLayoutFromActiveTab();

  
  
  
  void LayoutByTabOffsetAfter(int index);

  
  
  void LayoutByTabOffsetBefore(int index);

  
  
  void LayoutUsingCurrentAfter(int index);
  void LayoutUsingCurrentBefore(int index);

  void PushTabsAfter(int index, int delta);
  void PushTabsBefore(int index, int delta);

  
  
  void LayoutForDragAfter(int index);
  void LayoutForDragBefore(int index);

  
  
  
  void ExpandTabsBefore(int index, int delta);
  void ExpandTabsAfter(int index, int delta);

  
  
  
  
  void AdjustStackedTabs();
  void AdjustLeadingStackedTabs();
  void AdjustTrailingStackedTabs();

  
  void SetIdealBoundsAt(int index, int x);

  
  
  int GetMinX(int index) const;

  
  
  int GetMaxX(int index) const;

  
  int GetMinDragX(int index) const;
  int GetMaxDragX(int index) const;

  
  
  
  int GetMinXCompressed(int index) const;

  
  int width_for_count(int count) const {
    return (count * size_.width()) + (std::max(count - 1, 0) * padding_);
  }

  
  int stacked_padding_for_count(int count) const {
    return std::min(count, max_stacked_count_) * stacked_padding_;
  }

  
  int max_stacked_width() const {
    return stacked_padding_ * max_stacked_count_;
  }

  int ideal_x(int index) const { return view_model_->ideal_bounds(index).x(); }

  
  bool requires_stacking() const {
    return tab_count() != mini_tab_count_ &&
        x_ + width_for_count(tab_count() - mini_tab_count_) > width_;
  }

  
  int tab_count() const { return view_model_->view_size(); }

  
  int normal_tab_count() const { return tab_count() - mini_tab_count_; }

  
  int tab_offset() const { return size_.width() + padding_; }

  
  const gfx::Size size_;

  
  const int padding_;

  
  const int stacked_padding_;

  
  const int max_stacked_count_;

  
  views::ViewModel* view_model_;

  
  int x_;

  
  int width_;

  
  int mini_tab_count_;

  
  int mini_tab_to_non_mini_tab_;

  
  int active_index_;

  
  
  int first_tab_x_;

  DISALLOW_COPY_AND_ASSIGN(StackedTabStripLayout);
};

#endif  
