// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/tabs/tab_strip_layout_type.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "chrome/browser/ui/views/tabs/tab_controller.h"
#include "ui/gfx/animation/animation_container.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/mouse_watcher.h"
#include "ui/views/view.h"
#include "ui/views/view_model.h"

class NewTabButton;
class StackedTabStripLayout;
class Tab;
class TabDragController;
class TabStripController;
class TabStripObserver;

namespace ui {
class ListSelectionModel;
}

namespace views {
class ImageView;
}

class TabStrip : public views::View,
                 public views::ButtonListener,
                 public views::MouseWatcherListener,
                 public TabController {
 public:
  static const char kViewClassName[];

  explicit TabStrip(TabStripController* controller);
  virtual ~TabStrip();

  
  void AddObserver(TabStripObserver* observer);
  void RemoveObserver(TabStripObserver* observer);

  
  
  
  void SetLayoutType(TabStripLayoutType layout_type, bool adjust_layout);
  TabStripLayoutType layout_type() const { return layout_type_; }

  
  gfx::Rect GetNewTabButtonBounds();

  
  
  bool SizeTabButtonToTopOfTabStrip();

  
  void StartHighlight(int model_index);

  
  void StopAllHighlighting();

  
  void AddTabAt(int model_index, const TabRendererData& data, bool is_active);

  
  void MoveTab(int from_model_index,
               int to_model_index,
               const TabRendererData& data);

  
  void RemoveTabAt(int model_index);

  
  void SetTabData(int model_index, const TabRendererData& data);

  
  
  
  
  void PrepareForCloseAt(int model_index, CloseTabSource source);

  
  
  void SetSelection(const ui::ListSelectionModel& old_selection,
                    const ui::ListSelectionModel& new_selection);

  
  void TabTitleChangedNotLoading(int model_index);

  
  const gfx::Rect& ideal_bounds(int tab_data_index) {
    return tabs_.ideal_bounds(tab_data_index);
  }

  
  Tab* tab_at(int index) const;

  
  
  int GetModelIndexOfTab(const Tab* tab) const;

  
  int tab_count() const { return tabs_.view_size(); }

  
  int GetModelCount() const;

  
  bool IsValidModelIndex(int model_index) const;

  TabStripController* controller() const { return controller_.get(); }

  
  
  Tab* CreateTabForDragging();

  
  bool IsDragSessionActive() const;

  
  bool IsActiveDropTarget() const;

  
  
  
  bool IsTabStripEditable() const;

  
  
  bool IsTabStripCloseable() const;

  
  
  void UpdateLoadingAnimations();

  
  
  bool IsPositionInWindowCaption(const gfx::Point& point);

  
  
  bool IsRectInWindowCaption(const gfx::Rect& rect);

  
  void SetBackgroundOffset(const gfx::Point& offset);

  
  views::View* newtab_button();

  
  void SetImmersiveStyle(bool enable);

  
  
  bool IsAnimating() const;

  
  
  void StopAnimating(bool layout);

  
  void FileSupported(const GURL& url, bool supported);

  
  virtual const ui::ListSelectionModel& GetSelectionModel() OVERRIDE;
  virtual bool SupportsMultipleSelection() OVERRIDE;
  virtual void SelectTab(Tab* tab) OVERRIDE;
  virtual void ExtendSelectionTo(Tab* tab) OVERRIDE;
  virtual void ToggleSelected(Tab* tab) OVERRIDE;
  virtual void AddSelectionFromAnchorTo(Tab* tab) OVERRIDE;
  virtual void CloseTab(Tab* tab, CloseTabSource source) OVERRIDE;
  virtual void ShowContextMenuForTab(Tab* tab,
                                     const gfx::Point& p,
                                     ui::MenuSourceType source_type) OVERRIDE;
  virtual bool IsActiveTab(const Tab* tab) const OVERRIDE;
  virtual bool IsTabSelected(const Tab* tab) const OVERRIDE;
  virtual bool IsTabPinned(const Tab* tab) const OVERRIDE;
  virtual void MaybeStartDrag(
      Tab* tab,
      const ui::LocatedEvent& event,
      const ui::ListSelectionModel& original_selection) OVERRIDE;
  virtual void ContinueDrag(views::View* view,
                            const ui::LocatedEvent& event) OVERRIDE;
  virtual bool EndDrag(EndDragReason reason) OVERRIDE;
  virtual Tab* GetTabAt(Tab* tab,
                        const gfx::Point& tab_in_tab_coordinates) OVERRIDE;
  virtual void OnMouseEventInTab(views::View* source,
                                 const ui::MouseEvent& event) OVERRIDE;
  virtual bool ShouldPaintTab(const Tab* tab, gfx::Rect* clip) OVERRIDE;
  virtual bool IsImmersiveStyle() const OVERRIDE;

  
  virtual void MouseMovedOutOfHost() OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  
  
  virtual void OnDragEntered(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual views::View* GetEventHandlerForRect(const gfx::Rect& rect) OVERRIDE;
  virtual views::View* GetTooltipHandlerForPoint(
      const gfx::Point& point) OVERRIDE;

  
  static int GetImmersiveHeight();

 protected:
  
  static const int kMiniToNonMiniGap;

  void set_ideal_bounds(int index, const gfx::Rect& bounds) {
    tabs_.set_ideal_bounds(index, bounds);
  }

  
  int GetMiniTabCount() const;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual const views::View* GetViewByID(int id) const OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  typedef std::map<int, std::vector<Tab*> > TabsClosingMap;

  class RemoveTabDelegate;

  friend class TabDragController;
  friend class TabDragControllerTest;
  FRIEND_TEST_ALL_PREFIXES(TabDragControllerTest, GestureEndShouldEndDragTest);
  friend class TabStripTest;
  FRIEND_TEST_ALL_PREFIXES(TabStripTest, TabHitTestMaskWhenStacked);
  FRIEND_TEST_ALL_PREFIXES(TabStripTest, ClippedTabCloseButton);

  
  
  struct DropInfo {
    DropInfo(int drop_index,
             bool drop_before,
             bool point_down,
             views::Widget* context);
    ~DropInfo();

    
    
    
    
    int drop_index;
    bool drop_before;

    
    
    
    bool point_down;

    
    views::Widget* arrow_window;
    views::ImageView* arrow_view;

    
    GURL url;

    
    bool file_supported;

   private:
    DISALLOW_COPY_AND_ASSIGN(DropInfo);
  };

  void Init();

  
  Tab* CreateTab();

  
  void StartInsertTabAnimation(int model_index);

  
  
  void StartMoveTabAnimation();

  
  void StartRemoveTabAnimation(int model_index);

  
  
  void ScheduleRemoveTabAnimation(Tab* tab);

  
  
  
  void AnimateToIdealBounds();

  
  bool ShouldHighlightCloseButtonAfterRemove();

  
  void DoLayout();

  
  
  void DragActiveTab(const std::vector<int>& initial_positions, int delta);

  
  void SetIdealBoundsFromPositions(const std::vector<int>& positions);

  
  
  
  void StackDraggedTabs(int delta);

  
  bool IsStackingDraggedTabs() const;

  
  
  
  void LayoutDraggedTabsAt(const std::vector<Tab*>& tabs,
                           Tab* active_tab,
                           const gfx::Point& location,
                           bool initial_drag);

  
  
  void CalculateBoundsForDraggedTabs(const std::vector<Tab*>& tabs,
                                     std::vector<gfx::Rect>* bounds);

  
  
  int GetSizeNeededForTabs(const std::vector<Tab*>& tabs);

  
  
  void RemoveTabFromViewModel(int index);

  
  
  void RemoveAndDeleteTab(Tab* tab);

  
  
  void UpdateTabsClosingMap(int index, int delta);

  
  void StartedDraggingTabs(const std::vector<Tab*>& tabs);

  
  void DraggedTabsDetached();

  
  
  
  
  void StoppedDraggingTabs(const std::vector<Tab*>& tabs,
                           const std::vector<int>& initial_positions,
                           bool move_only,
                           bool completed);

  
  
  void StoppedDraggingTab(Tab* tab, bool* is_first_tab);

  
  void OwnDragController(TabDragController* controller);

  
  
  void DestroyDragController();

  
  TabDragController* ReleaseDragController();

  
  void PaintClosingTabs(gfx::Canvas* canvas, int index);

  
  
  void UpdateLayoutTypeFromMouseEvent(views::View* source,
                                      const ui::MouseEvent& event);

  

  
  void GetCurrentTabWidths(double* unselected_width,
                           double* selected_width) const;

  
  
  
  
  
  
  void GetDesiredTabWidths(int tab_count,
                           int mini_tab_count,
                           double* unselected_width,
                           double* selected_width) const;

  
  void ResizeLayoutTabs();

  
  
  void ResizeLayoutTabsFromTouch();

  
  void StartResizeLayoutTabsFromTouchTimer();

  
  void SetTabBoundsForDrag(const std::vector<gfx::Rect>& tab_bounds);

  
  
  void AddMessageLoopObserver();
  void RemoveMessageLoopObserver();

  

  
  
  
  gfx::Rect GetDropBounds(int drop_index, bool drop_before, bool* is_beneath);

  
  void UpdateDropIndex(const ui::DropTargetEvent& event);

  
  void SetDropIndex(int tab_data_index, bool drop_before);

  
  
  int GetDropEffect(const ui::DropTargetEvent& event);

  
  
  static gfx::ImageSkia* GetDropArrowImage(bool is_down);

  

  
  void PrepareForAnimation();

  
  
  void GenerateIdealBounds();

  
  
  
  int GenerateIdealBoundsForMiniTabs(int* first_non_mini_index);

  
  static int new_tab_button_width();

  
  
  static int button_v_offset();

  
  
  int tab_area_width() const;

  
  void StartResizeLayoutAnimation();
  void StartMiniTabAnimation();
  void StartMouseInitiatedRemoveTabAnimation(int model_index);

  
  
  bool IsPointInTab(Tab* tab, const gfx::Point& point_in_tabstrip_coords);

  

  
  int GetStartXForNormalTabs() const;

  
  
  Tab* FindTabForEvent(const gfx::Point& point);

  
  
  Tab* FindTabForEventFrom(const gfx::Point& point, int start, int delta);

  
  
  
  
  
  views::View* FindTabHitByPoint(const gfx::Point& point);

  
  std::vector<int> GetTabXCoordinates();

  
  void SwapLayoutIfNecessary();

  
  bool NeedsTouchLayout() const;

  
  
  
  void SetResetToShrinkOnExit(bool value);

  
  bool GetAdjustLayout() const;

  

  
  
  
  
  
  
  
  views::ViewModel tabs_;
  TabsClosingMap tabs_closing_map_;

  scoped_ptr<TabStripController> controller_;

  
  NewTabButton* newtab_button_;

  
  gfx::Rect newtab_button_bounds_;

  
  
  
  
  double current_unselected_width_;
  double current_selected_width_;

  
  
  
  
  
  
  
  int available_width_for_tabs_;

  
  
  bool in_tab_close_;

  
  scoped_ptr<DropInfo> drop_info_;

  
  
  scoped_refptr<gfx::AnimationContainer> animation_container_;

  
  
  
  
  scoped_ptr<views::MouseWatcher> mouse_watcher_;

  
  
  scoped_ptr<TabDragController> drag_controller_;

  views::BoundsAnimator bounds_animator_;

  
  gfx::Size last_layout_size_;

  TabStripLayoutType layout_type_;

  
  bool adjust_layout_;

  
  scoped_ptr<StackedTabStripLayout> touch_layout_;

  
  
  bool reset_to_shrink_on_exit_;

  
  gfx::Point last_mouse_move_location_;

  
  base::TimeTicks last_mouse_move_time_;

  
  int mouse_move_count_;

  
  
  base::OneShotTimer<TabStrip> resize_layout_timer_;

  
  bool immersive_style_;

  
  typedef ObserverList<TabStripObserver> TabStripObservers;
  TabStripObservers observers_;

  DISALLOW_COPY_AND_ASSIGN(TabStrip);
};

#endif  
