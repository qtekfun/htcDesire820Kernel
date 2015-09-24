// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_DRAG_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_DRAG_CONTROLLER_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/tabs/dock_info.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/views/tabs/tab_strip_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/base/models/list_selection_model.h"
#include "ui/gfx/rect.h"
#include "ui/views/widget/widget_observer.h"

namespace gfx {
class Screen;
}
namespace ui {
class ListSelectionModel;
}
namespace views {
class View;
}
class Browser;
class DraggedTabView;
class Tab;
struct TabRendererData;
class TabStrip;
class TabStripModel;

class TabDragController : public content::WebContentsDelegate,
                          public content::NotificationObserver,
                          public base::MessageLoopForUI::Observer,
                          public views::WidgetObserver,
                          public TabStripModelObserver {
 public:
  enum DetachBehavior {
    DETACHABLE,
    NOT_DETACHABLE
  };

  
  enum MoveBehavior {
    
    
    MOVE_VISIBILE_TABS,

    
    REORDER
  };

  
  enum EventSource {
    EVENT_SOURCE_MOUSE,
    EVENT_SOURCE_TOUCH,
  };

  
  static const int kTouchVerticalDetachMagnetism;
  static const int kVerticalDetachMagnetism;

  TabDragController();
  virtual ~TabDragController();

  
  
  
  
  
  
  
  
  void Init(TabStrip* source_tabstrip,
            Tab* source_tab,
            const std::vector<Tab*>& tabs,
            const gfx::Point& mouse_offset,
            int source_tab_offset,
            const ui::ListSelectionModel& initial_selection_model,
            DetachBehavior detach_behavior,
            MoveBehavior move_behavior,
            EventSource event_source);

  
  
  
  
  static bool IsAttachedTo(const TabStrip* tab_strip);

  
  static bool IsActive();

  
  
  static bool ShouldDetachIntoNewBrowser();

  
  void SetMoveBehavior(MoveBehavior behavior);
  MoveBehavior move_behavior() const { return move_behavior_; }

  EventSource event_source() const { return event_source_; }

  
  bool active() const { return active_; }
  const TabStrip* attached_tabstrip() const { return attached_tabstrip_; }

  
  bool started_drag() const { return started_drag_; }

  
  bool is_mutating() const { return is_mutating_; }

  
  
  bool is_dragging_window() const { return is_dragging_window_; }

  
  void Drag(const gfx::Point& point_in_screen);

  
  void EndDrag(EndDragReason reason);

 private:
  class DockDisplayer;
  friend class DockDisplayer;

  typedef std::set<gfx::NativeView> DockWindows;

  
  static const int kMovedMouseLeft  = 1 << 0;
  static const int kMovedMouseRight = 1 << 1;

  
  enum EndDragType {
    
    NORMAL,

    
    CANCELED,

    
    TAB_DESTROYED
  };

  
  enum ReleaseCapture {
    RELEASE_CAPTURE,
    DONT_RELEASE_CAPTURE,
  };

  
  enum EndRunLoopBehavior {
    
    END_RUN_LOOP_STOP_DRAGGING,

    
    END_RUN_LOOP_CONTINUE_DRAGGING
  };

  
  enum DetachPosition {
    DETACH_BEFORE,
    DETACH_AFTER,
    DETACH_ABOVE_OR_BELOW
  };

  
  enum DragBrowserResultType {
    
    
    
    DRAG_BROWSER_RESULT_STOP,

    
    DRAG_BROWSER_RESULT_CONTINUE,
  };

  
  struct TabDragData {
    TabDragData();
    ~TabDragData();

    
    content::WebContents* contents;

    
    
    
    content::WebContentsDelegate* original_delegate;

    
    
    int source_model_index;

    
    Tab* attached_tab;

    
    bool pinned;
  };

  typedef std::vector<TabDragData> DragData;

  
  
  void InitTabDragData(Tab* tab, TabDragData* drag_data);

  
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual void LoadingStateChanged(content::WebContents* source) OVERRIDE;
  virtual bool ShouldSuppressDialogs() OVERRIDE;
  virtual content::JavaScriptDialogManager*
      GetJavaScriptDialogManager() OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;

  
  virtual void OnWidgetBoundsChanged(views::Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
  virtual void TabStripEmpty() OVERRIDE;

  
  
  void InitWindowCreatePoint();

  
  
  gfx::Point GetWindowCreatePoint(const gfx::Point& origin) const;

  void UpdateDockInfo(const gfx::Point& point_in_screen);

  
  
  void SaveFocus();

  
  
  void RestoreFocus();

  
  
  bool CanStartDrag(const gfx::Point& point_in_screen) const;

  
  
  void ContinueDragging(const gfx::Point& point_in_screen);

  
  
  
  DragBrowserResultType DragBrowserToNewTabStrip(
      TabStrip* target_tabstrip,
      const gfx::Point& point_in_screen);

  
  
  void DragActiveTabStacked(const gfx::Point& point_in_screen);

  
  
  void MoveAttachedToNextStackedIndex(const gfx::Point& point_in_screen);
  void MoveAttachedToPreviousStackedIndex(const gfx::Point& point_in_screen);

  
  void MoveAttached(const gfx::Point& point_in_screen);

  
  void MoveDetached(const gfx::Point& point_in_screen);

  
  
  void StartMoveStackedTimerIfNecessary(
      const gfx::Point& point_in_screen,
      int delay_ms);

  
  
  TabStrip* GetTabStripForWindow(gfx::NativeWindow window);

  
  
  TabStrip* GetTargetTabStripForPoint(const gfx::Point& point_in_screen);

  
  
  bool DoesTabStripContain(TabStrip* tabstrip,
                           const gfx::Point& point_in_screen) const;

  
  
  DetachPosition GetDetachPosition(const gfx::Point& point_in_screen);

  DockInfo GetDockInfoAtPoint(const gfx::Point& point_in_screen);

  
  void Attach(TabStrip* attached_tabstrip, const gfx::Point& point_in_screen);

  
  void Detach(ReleaseCapture release_capture);

  
  
  void DetachIntoNewBrowserAndRunMoveLoop(const gfx::Point& point_in_screen);

  
  
  
  
  void RunMoveLoop(const gfx::Vector2d& drag_offset);

  
  
  
  int GetInsertionIndexFrom(const gfx::Rect& dragged_bounds,
                            int start,
                            int delta) const;

  
  
  
  
  
  int GetInsertionIndexForDraggedBounds(const gfx::Rect& dragged_bounds) const;

  
  
  bool ShouldDragToNextStackedTab(const gfx::Rect& dragged_bounds,
                                  int index) const;

  
  
  bool ShouldDragToPreviousStackedTab(const gfx::Rect& dragged_bounds,
                                      int index) const;

  
  int GetInsertionIndexForDraggedBoundsStacked(
      const gfx::Rect& dragged_bounds) const;

  
  
  
  gfx::Rect GetDraggedViewTabStripBounds(const gfx::Point& tab_strip_point);

  
  
  gfx::Point GetAttachedDragPoint(const gfx::Point& point_in_screen);

  
  
  std::vector<Tab*> GetTabsMatchingDraggedContents(TabStrip* tabstrip);

  
  std::vector<gfx::Rect> CalculateBoundsForDraggedTabs();

  
  
  void EndDragImpl(EndDragType how_end);

  
  void RevertDrag();

  
  void RevertDragAt(size_t drag_index);

  
  
  
  void ResetSelection(TabStripModel* model);

  
  void CompleteDrag();

  
  void ResetDelegates();

  
  void CreateDraggedView(const std::vector<TabRendererData>& data,
                         const std::vector<gfx::Rect>& renderer_bounds);

  
  gfx::Rect GetViewScreenBounds(views::View* tabstrip) const;

  
  
  void HideFrame();

  
  void CleanUpHiddenFrame();

  void DockDisplayerDestroyed(DockDisplayer* controller);

  void BringWindowUnderPointToFront(const gfx::Point& point_in_screen);

  
  
  TabDragData* source_tab_drag_data() {
    return &(drag_data_[source_tab_index_]);
  }

  
  content::WebContents* source_dragged_contents() {
    return source_tab_drag_data()->contents;
  }

  
  views::Widget* GetAttachedBrowserWidget();

  
  
  bool AreTabsConsecutive();

  
  
  
  
  
  
  gfx::Rect CalculateDraggedBrowserBounds(TabStrip* source,
                                          const gfx::Point& point_in_screen,
                                          std::vector<gfx::Rect>* drag_bounds);

  
  
  
  
  
  void AdjustBrowserAndTabBoundsForDrag(int last_tabstrip_width,
                                        const gfx::Point& point_in_screen,
                                        std::vector<gfx::Rect>* drag_bounds);

  
  Browser* CreateBrowserForDrag(TabStrip* source,
                                const gfx::Point& point_in_screen,
                                gfx::Vector2d* drag_offset,
                                std::vector<gfx::Rect>* drag_bounds);

  
  TabStripModel* GetModel(TabStrip* tabstrip) const;

  
  
  gfx::Point GetCursorScreenPoint();

  
  
  gfx::Vector2d GetWindowOffset(const gfx::Point& point_in_screen);

  
  bool move_only() const {
    return (move_behavior_ == MOVE_VISIBILE_TABS) != 0;
  }

  
  bool detach_into_browser_;

  
  content::NotificationRegistrar registrar_;

  EventSource event_source_;

  
  TabStrip* source_tabstrip_;

  
  
  TabStrip* attached_tabstrip_;

  
  
  gfx::Screen* screen_;

  
  
  
  chrome::HostDesktopType host_desktop_type_;

  
  scoped_ptr<DraggedTabView> view_;

  
  
  
  gfx::Point start_point_in_screen_;

  
  
  
  
  gfx::Point mouse_offset_;

  
  
  float offset_to_width_ratio_;

  
  
  
  
  gfx::Point window_create_point_;

  
  
  gfx::Point first_source_tab_point_;

  
  
  
  
  
  gfx::Rect restore_bounds_;

  
  
  
  const int old_focused_view_id_;

  
  
  int last_move_screen_loc_;

  DockInfo dock_info_;

  DockWindows dock_windows_;

  std::vector<DockDisplayer*> dock_controllers_;

  
  
  
  base::OneShotTimer<TabDragController> bring_to_front_timer_;

  
  
  base::OneShotTimer<TabDragController> move_stacked_timer_;

  
  bool started_drag_;

  
  bool active_;

  DragData drag_data_;

  
  size_t source_tab_index_;

  
  bool initial_move_;

  
  
  ui::ListSelectionModel initial_selection_model_;

  
  ui::ListSelectionModel selection_model_before_attach_;

  
  
  std::vector<int> initial_tab_positions_;

  DetachBehavior detach_behavior_;
  MoveBehavior move_behavior_;

  
  
  
  int mouse_move_direction_;

  
  gfx::Point last_point_in_screen_;

  
  

  
  bool is_dragging_window_;

  
  
  bool is_dragging_new_browser_;

  
  bool was_source_maximized_;

  
  bool was_source_fullscreen_;

  EndRunLoopBehavior end_run_loop_behavior_;

  
  bool waiting_for_run_loop_to_exit_;

  
  TabStrip* tab_strip_to_attach_to_after_exit_;

  
  views::Widget* move_loop_widget_;

  
  bool is_mutating_;

  
  
  
  
  
  
  int attach_x_;
  int attach_index_;

  base::WeakPtrFactory<TabDragController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabDragController);
};

#endif  
