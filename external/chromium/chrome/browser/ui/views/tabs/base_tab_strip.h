// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_BASE_TAB_STRIP_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_BASE_TAB_STRIP_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/tabs/abstract_tab_strip_view.h"
#include "chrome/browser/ui/views/tabs/base_tab.h"
#include "chrome/browser/ui/views/tabs/tab_controller.h"
#include "views/animation/bounds_animator.h"
#include "views/view.h"

class BaseTab;
class DraggedTabController;
class TabStripController;

class BaseTabStrip : public AbstractTabStripView,
                     public TabController {
 public:
  enum Type {
    HORIZONTAL_TAB_STRIP,
    VERTICAL_TAB_STRIP
  };

  BaseTabStrip(TabStripController* controller, Type type);
  virtual ~BaseTabStrip();

  Type type() const { return type_; }

  
  virtual void StartHighlight(int model_index) = 0;

  
  virtual void StopAllHighlighting() = 0;

  
  const gfx::Rect& ideal_bounds(int tab_data_index) {
    return tab_data_[tab_data_index].ideal_bounds;
  }

  
  
  virtual BaseTab* CreateTabForDragging() = 0;

  
  void AddTabAt(int model_index, const TabRendererData& data);

  
  
  
  
  virtual void PrepareForCloseAt(int model_index) {}

  
  virtual void RemoveTabAt(int model_index) = 0;

  
  
  virtual void SelectTabAt(int old_model_index, int new_model_index) = 0;

  
  virtual void MoveTab(int from_model_index, int to_model_index);

  
  virtual void TabTitleChangedNotLoading(int model_index) = 0;

  
  virtual void SetTabData(int model_index, const TabRendererData& data);

  
  virtual BaseTab* GetBaseTabAtModelIndex(int model_index) const;

  
  BaseTab* base_tab_at_tab_index(int tab_index) const {
    return tab_data_[tab_index].tab;
  }

  
  
  int GetModelIndexOfBaseTab(const BaseTab* tab) const;

  
  
  
  
  int tab_count() const { return static_cast<int>(tab_data_.size()); }

  
  int GetModelCount() const;

  
  bool IsValidModelIndex(int model_index) const;

  
  
  
  int ModelIndexToTabIndex(int model_index) const;

  TabStripController* controller() const { return controller_.get(); }

  
  bool IsDragSessionActive() const;

  
  bool IsActiveDropTarget() const;

  
  virtual bool IsTabStripEditable() const OVERRIDE;
  virtual bool IsTabStripCloseable() const OVERRIDE;
  virtual void UpdateLoadingAnimations() OVERRIDE;

  
  virtual void SelectTab(BaseTab* tab) OVERRIDE;
  virtual void ExtendSelectionTo(BaseTab* tab) OVERRIDE;
  virtual void ToggleSelected(BaseTab* tab) OVERRIDE;
  virtual void AddSelectionFromAnchorTo(BaseTab* tab) OVERRIDE;
  virtual void CloseTab(BaseTab* tab) OVERRIDE;
  virtual void ShowContextMenuForTab(BaseTab* tab,
                                     const gfx::Point& p) OVERRIDE;
  virtual bool IsActiveTab(const BaseTab* tab) const OVERRIDE;
  virtual bool IsTabSelected(const BaseTab* tab) const OVERRIDE;
  virtual bool IsTabPinned(const BaseTab* tab) const OVERRIDE;
  virtual bool IsTabCloseable(const BaseTab* tab) const OVERRIDE;
  virtual void MaybeStartDrag(BaseTab* tab,
                              const views::MouseEvent& event) OVERRIDE;
  virtual void ContinueDrag(const views::MouseEvent& event) OVERRIDE;
  virtual bool EndDrag(bool canceled) OVERRIDE;
  virtual BaseTab* GetTabAt(BaseTab* tab,
                            const gfx::Point& tab_in_tab_coordinates) OVERRIDE;

  
  virtual void Layout() OVERRIDE;

 protected:
  
  struct TabData {
    BaseTab* tab;
    gfx::Rect ideal_bounds;
  };

  
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;

  
  virtual BaseTab* CreateTab() = 0;

  
  
  virtual void StartInsertTabAnimation(int model_index) = 0;

  
  
  virtual void StartMoveTabAnimation();

  
  virtual void StartRemoveTabAnimation(int model_index);

  
  virtual void StartMiniTabAnimation();

  
  virtual bool ShouldHighlightCloseButtonAfterRemove();

  
  
  
  virtual void AnimateToIdealBounds() = 0;

  
  
  void RemoveAndDeleteTab(BaseTab* tab);

  
  virtual void GenerateIdealBounds() = 0;

  
  
  
  virtual void LayoutDraggedTabsAt(const std::vector<BaseTab*>& tabs,
                                   BaseTab* active_tab,
                                   const gfx::Point& location,
                                   bool initial_drag) = 0;

  
  
  virtual void CalculateBoundsForDraggedTabs(
      const std::vector<BaseTab*>& tabs,
      std::vector<gfx::Rect>* bounds) = 0;

  void set_ideal_bounds(int index, const gfx::Rect& bounds) {
    tab_data_[index].ideal_bounds = bounds;
  }

  
  
  int TabIndexOfTab(BaseTab* tab) const;

  
  
  virtual void StopAnimating(bool layout);

  
  void DestroyDragController();

  
  void StartedDraggingTabs(const std::vector<BaseTab*>& tabs);
  void StoppedDraggingTabs(const std::vector<BaseTab*>& tabs);

  
  
  virtual int GetSizeNeededForTabs(const std::vector<BaseTab*>& tabs) = 0;

  
  bool attaching_dragged_tab() const { return attaching_dragged_tab_; }

  views::BoundsAnimator& bounds_animator() { return bounds_animator_; }

  
  virtual void PrepareForAnimation();

  
  
  ui::AnimationDelegate* CreateRemoveTabDelegate(BaseTab* tab);

  
  virtual void DoLayout();

  
  
  bool IsAnimating() const;

  
  BaseTab* GetTabAtLocal(const gfx::Point& local_point);

 private:
  class RemoveTabDelegate;

  friend class DraggedTabController;

  
  
  void StoppedDraggingTab(BaseTab* tab, bool* is_first_tab);

  
  void set_attaching_dragged_tab(bool value) { attaching_dragged_tab_ = value; }

  scoped_ptr<TabStripController> controller_;

  const Type type_;

  std::vector<TabData> tab_data_;

  
  
  scoped_ptr<DraggedTabController> drag_controller_;

  
  
  bool attaching_dragged_tab_;

  views::BoundsAnimator bounds_animator_;

  
  gfx::Size last_layout_size_;
};

#endif  
