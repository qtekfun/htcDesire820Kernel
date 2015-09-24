// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_SIDE_TAB_STRIP_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_SIDE_TAB_STRIP_H_
#pragma once

#include "chrome/browser/ui/views/tabs/base_tab_strip.h"
#include "views/controls/button/button.h"

struct TabRendererData;

class SideTabStrip : public BaseTabStrip, public views::ButtonListener {
 public:
  
  static const int kTabStripInset;

  explicit SideTabStrip(TabStripController* controller);
  virtual ~SideTabStrip();

  
  virtual bool IsPositionInWindowCaption(const gfx::Point& point) OVERRIDE;
  virtual void SetBackgroundOffset(const gfx::Point& offset) OVERRIDE;

  
  virtual void StartHighlight(int model_index) OVERRIDE;
  virtual void StopAllHighlighting() OVERRIDE;
  virtual BaseTab* CreateTabForDragging() OVERRIDE;
  virtual void RemoveTabAt(int model_index) OVERRIDE;
  virtual void SelectTabAt(int old_model_index, int new_model_index) OVERRIDE;
  virtual void TabTitleChangedNotLoading(int model_index) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual views::View* GetEventHandlerForPoint(
      const gfx::Point& point) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

 protected:
  
  virtual BaseTab* CreateTab() OVERRIDE;
  virtual void GenerateIdealBounds() OVERRIDE;
  virtual void StartInsertTabAnimation(int model_index) OVERRIDE;
  virtual void AnimateToIdealBounds() OVERRIDE;
  virtual void DoLayout() OVERRIDE;
  virtual void LayoutDraggedTabsAt(const std::vector<BaseTab*>& tabs,
                                   BaseTab* active_tab,
                                   const gfx::Point& location,
                                   bool initial_drag) OVERRIDE;
  virtual void CalculateBoundsForDraggedTabs(
      const std::vector<BaseTab*>& tabs,
      std::vector<gfx::Rect>* bounds) OVERRIDE;
  virtual int GetSizeNeededForTabs(const std::vector<BaseTab*>& tabs) OVERRIDE;

  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  
  void SetFirstTabYOffset(int new_offset);

  
  int GetMaxOffset() const;

  
  int GetMaxTabY() const;

  
  void MakeTabVisible(int tab_index);

  
  views::View* newtab_button_;

  
  gfx::Rect newtab_button_bounds_;

  
  views::View* scroll_up_button_;
  views::View* scroll_down_button_;

  
  
  views::View* separator_;

  
  gfx::Rect separator_bounds_;

  
  
  int first_tab_y_offset_;

  
  
  int ideal_height_;

  DISALLOW_COPY_AND_ASSIGN(SideTabStrip);
};

#endif  
