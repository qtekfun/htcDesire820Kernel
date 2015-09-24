// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/timer.h"
#include "chrome/browser/ui/views/tabs/base_tab_strip.h"
#include "ui/base/animation/animation_container.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "views/controls/button/image_button.h"
#include "views/mouse_watcher.h"

class Tab;

namespace views {
class ImageView;
#if defined(OS_LINUX)
class WidgetGtk;
#elif defined(OS_WIN)
class WidgetWin;
#endif
}

class TabStrip : public BaseTabStrip,
                 public views::ButtonListener,
                 public views::MouseWatcherListener {
 public:
  explicit TabStrip(TabStripController* controller);
  virtual ~TabStrip();

  
  void InitTabStripButtons();

  
  gfx::Rect GetNewTabButtonBounds();

  
  virtual void MouseMovedOutOfView() OVERRIDE;

  
  virtual bool IsPositionInWindowCaption(const gfx::Point& point) OVERRIDE;
  virtual void SetBackgroundOffset(const gfx::Point& offset) OVERRIDE;

  
  virtual void PrepareForCloseAt(int model_index) OVERRIDE;
  virtual void RemoveTabAt(int model_index) OVERRIDE;
  virtual void SelectTabAt(int old_model_index, int new_model_index) OVERRIDE;
  virtual void TabTitleChangedNotLoading(int model_index) OVERRIDE;
  virtual void StartHighlight(int model_index) OVERRIDE;
  virtual void StopAllHighlighting() OVERRIDE;
  virtual BaseTab* CreateTabForDragging() OVERRIDE;

  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual const views::View* GetViewByID(int id) const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  
  
  virtual void OnDragEntered(const views::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const views::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const views::DropTargetEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual views::View* GetEventHandlerForPoint(const gfx::Point& point)
      OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

 protected:
  
  virtual BaseTab* CreateTab() OVERRIDE;
  virtual void StartInsertTabAnimation(int model_index) OVERRIDE;
  virtual void AnimateToIdealBounds() OVERRIDE;
  virtual bool ShouldHighlightCloseButtonAfterRemove() OVERRIDE;
  virtual void DoLayout() OVERRIDE;
  virtual void LayoutDraggedTabsAt(const std::vector<BaseTab*>& tabs,
                                   BaseTab* active_tab,
                                   const gfx::Point& location,
                                   bool initial_drag) OVERRIDE;
  virtual void CalculateBoundsForDraggedTabs(
      const std::vector<BaseTab*>& tabs,
      std::vector<gfx::Rect>* bounds) OVERRIDE;
  virtual int GetSizeNeededForTabs(const std::vector<BaseTab*>& tabs) OVERRIDE;

  
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event)
      OVERRIDE;

  
  static const int mini_to_non_mini_gap_;

 private:
  friend class DraggedTabController;

  
  
  struct DropInfo {
    DropInfo(int index, bool drop_before, bool paint_down);
    ~DropInfo();

    
    
    
    
    int drop_index;
    bool drop_before;

    
    
    
    bool point_down;

    
    views::Widget* arrow_window;
    views::ImageView* arrow_view;

   private:
    DISALLOW_COPY_AND_ASSIGN(DropInfo);
  };

  void Init();

  
  void LoadNewTabButtonImage();

  
  
  Tab* GetTabAtTabDataIndex(int tab_data_index) const;

  
  
  
  
  
  
  
  Tab* GetTabAtModelIndex(int model_index) const;

  
  int GetMiniTabCount() const;

  

  
  void GetCurrentTabWidths(double* unselected_width,
                           double* selected_width) const;

  
  
  
  
  
  
  void GetDesiredTabWidths(int tab_count,
                           int mini_tab_count,
                           double* unselected_width,
                           double* selected_width) const;

  
  void ResizeLayoutTabs();

  
  
  void AddMessageLoopObserver();
  void RemoveMessageLoopObserver();

  

  
  
  
  gfx::Rect GetDropBounds(int drop_index, bool drop_before, bool* is_beneath);

  
  void UpdateDropIndex(const views::DropTargetEvent& event);

  
  void SetDropIndex(int tab_data_index, bool drop_before);

  
  
  int GetDropEffect(const views::DropTargetEvent& event);

  
  
  static SkBitmap* GetDropArrowImage(bool is_down);

  

  
  
  
  
  virtual void GenerateIdealBounds();

  
  void StartResizeLayoutAnimation();
  virtual void StartMiniTabAnimation();
  void StartMouseInitiatedRemoveTabAnimation(int model_index);

  
  int GetAvailableWidthForTabs(Tab* last_tab) const;

  
  
  bool IsPointInTab(Tab* tab, const gfx::Point& point_in_tabstrip_coords);

  

  
  views::ImageButton* newtab_button_;

  
  gfx::Rect newtab_button_bounds_;

  
  
  
  
  double current_unselected_width_;
  double current_selected_width_;

  
  
  
  
  
  
  
  int available_width_for_tabs_;

  
  
  bool in_tab_close_;

  
  
  
  
  static const int kNewTabButtonWidth = 28;
  static const int kNewTabButtonHeight = 18;

  
  scoped_ptr<DropInfo> drop_info_;

  
  
  scoped_refptr<ui::AnimationContainer> animation_container_;

  
  base::OneShotTimer<TabStrip> new_tab_timer_;

  scoped_ptr<views::MouseWatcher> mouse_watcher_;

  DISALLOW_COPY_AND_ASSIGN(TabStrip);
};

#endif  
