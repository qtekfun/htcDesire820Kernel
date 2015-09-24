// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOUCH_TABS_TOUCH_TAB_STRIP_H_
#define CHROME_BROWSER_UI_TOUCH_TABS_TOUCH_TAB_STRIP_H_
#pragma once

#include "chrome/browser/ui/views/tabs/base_tab_strip.h"

class TouchTab;

class TouchTabStrip : public BaseTabStrip {
 public:
  explicit TouchTabStrip(TabStripController* controller);
  virtual ~TouchTabStrip();

  
  virtual bool IsPositionInWindowCaption(const gfx::Point& point);
  virtual void SetBackgroundOffset(const gfx::Point& offset);

  
  virtual void PrepareForCloseAt(int model_index);
  virtual void StartHighlight(int model_index);
  virtual void StopAllHighlighting();
  virtual BaseTab* CreateTabForDragging();
  virtual void RemoveTabAt(int model_index);
  virtual void SelectTabAt(int old_model_index, int new_model_index);
  virtual void TabTitleChangedNotLoading(int model_index);
  virtual BaseTab* CreateTab();
  virtual void StartInsertTabAnimation(int model_index);
  virtual void AnimateToIdealBounds();
  virtual bool ShouldHighlightCloseButtonAfterRemove();
  virtual void GenerateIdealBounds();
  virtual void LayoutDraggedTabsAt(const std::vector<BaseTab*>& tabs,
                                   BaseTab* active_tab,
                                   const gfx::Point& location,
                                   bool initial_drag);
  virtual void CalculateBoundsForDraggedTabs(
      const std::vector<BaseTab*>& tabs,
      std::vector<gfx::Rect>* bounds);
  virtual int GetSizeNeededForTabs(const std::vector<BaseTab*>& tabs);

  
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;

  
  
  TouchTab* GetTabAtTabDataIndex(int tab_data_index) const;

 private:
  void Init();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual views::View::TouchStatus OnTouchEvent(
      const views::TouchEvent& event) OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;

  
  
  void BeginScroll(const gfx::Point& point);

  
  
  
  void ContinueScroll(const gfx::Point& point);

  
  
  
  void EndScroll(const gfx::Point& point);

  
  
  void CancelScroll();

  
  
  void ScrollTo(int delta_x);

  
  
  bool in_tab_close_;

  
  base::Time last_tap_time_;

  
  View* last_tapped_view_;

  
  int initial_mouse_x_;

  
  int initial_scroll_offset_;

  
  
  int scroll_offset_;

  
  
  bool scrolling_;

  
  
  
  TouchTab* initial_tab_;

  
  
  int min_scroll_offset_;

  DISALLOW_COPY_AND_ASSIGN(TouchTabStrip);
};

#endif  
