// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_DRAGGED_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_DRAGGED_TAB_CONTROLLER_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/timer.h"
#include "chrome/browser/ui/tab_contents/tab_contents_wrapper.h"
#include "chrome/browser/ui/tabs/dock_info.h"
#include "content/browser/tab_contents/tab_contents_delegate.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/rect.h"

namespace views {
class View;
}
class BaseTab;
class BaseTabStrip;
class DraggedTabView;
class TabStripModel;

struct TabRendererData;

class DraggedTabController : public TabContentsDelegate,
                             public NotificationObserver,
                             public MessageLoopForUI::Observer {
 public:
  DraggedTabController();
  virtual ~DraggedTabController();

  
  
  
  
  
  
  
  void Init(BaseTabStrip* source_tabstrip,
            BaseTab* source_tab,
            const std::vector<BaseTab*>& tabs,
            const gfx::Point& mouse_offset,
            int source_tab_offset);

  
  
  
  
  static bool IsAttachedTo(BaseTabStrip* tab_strip);

  
  
  
  void Drag();

  
  
  
  
  void EndDrag(bool canceled);

  
  bool started_drag() const { return started_drag_; }

 private:
  class DockDisplayer;
  friend class DockDisplayer;

  typedef std::set<gfx::NativeView> DockWindows;

  
  enum EndDragType {
    
    NORMAL,

    
    CANCELED,

    
    TAB_DESTROYED
  };

  
  struct TabDragData {
    TabDragData();
    ~TabDragData();

    
    TabContentsWrapper* contents;

    
    
    
    TabContentsDelegate* original_delegate;

    
    
    int source_model_index;

    
    BaseTab* attached_tab;

    
    bool pinned;
  };

  typedef std::vector<TabDragData> DragData;

  
  
  void InitTabDragData(BaseTab* tab, TabDragData* drag_data);

  
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition) OVERRIDE;
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void AddNewContents(TabContents* source,
                              TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture) OVERRIDE;
  virtual void ActivateContents(TabContents* contents) OVERRIDE;
  virtual void DeactivateContents(TabContents* contents) OVERRIDE;
  virtual void LoadingStateChanged(TabContents* source) OVERRIDE;
  virtual void CloseContents(TabContents* source) OVERRIDE;
  virtual void MoveContents(TabContents* source,
                            const gfx::Rect& pos) OVERRIDE;
  virtual void UpdateTargetURL(TabContents* source, const GURL& url) OVERRIDE;
  virtual bool ShouldSuppressDialogs() OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
#if defined(OS_WIN)
  virtual void WillProcessMessage(const MSG& msg) OVERRIDE;
  virtual void DidProcessMessage(const MSG& msg) OVERRIDE;
#else
  virtual void WillProcessEvent(GdkEvent* event) OVERRIDE;
  virtual void DidProcessEvent(GdkEvent* event) OVERRIDE;
#endif

  
  
  void InitWindowCreatePoint();

  
  
  gfx::Point GetWindowCreatePoint() const;

  void UpdateDockInfo(const gfx::Point& screen_point);

  
  
  void SaveFocus();

  
  
  void RestoreFocus();

  
  
  bool CanStartDrag() const;

  
  
  void ContinueDragging();

  
  void MoveAttached(const gfx::Point& screen_point);

  
  void MoveDetached(const gfx::Point& screen_point);

  
  
  BaseTabStrip* GetTabStripForPoint(const gfx::Point& screen_point);

  DockInfo GetDockInfoAtPoint(const gfx::Point& screen_point);

  
  
  BaseTabStrip* GetTabStripIfItContains(BaseTabStrip* tabstrip,
                                        const gfx::Point& screen_point) const;

  
  void Attach(BaseTabStrip* attached_tabstrip, const gfx::Point& screen_point);

  
  void Detach();

  
  
  
  
  
  int GetInsertionIndexForDraggedBounds(const gfx::Rect& dragged_bounds) const;

  
  
  
  gfx::Rect GetDraggedViewTabStripBounds(const gfx::Point& tab_strip_point);

  
  
  gfx::Point GetAttachedDragPoint(const gfx::Point& screen_point);

  
  
  std::vector<BaseTab*> GetTabsMatchingDraggedContents(BaseTabStrip* tabstrip);

  
  
  void EndDragImpl(EndDragType how_end);

  
  void RevertDrag();

  
  void RevertDragAt(size_t drag_index);

  
  
  
  void ResetSelection(TabStripModel* model);

  
  void CompleteDrag();

  
  void ResetDelegates();

  
  void CreateDraggedView(const std::vector<TabRendererData>& data,
                         const std::vector<gfx::Rect>& renderer_bounds);

  
  gfx::Point GetCursorScreenPoint() const;

  
  gfx::Rect GetViewScreenBounds(views::View* tabstrip) const;

  
  
  void HideFrame();

  
  void CleanUpHiddenFrame();

  void DockDisplayerDestroyed(DockDisplayer* controller);

  void BringWindowUnderMouseToFront();

  
  
  TabDragData* source_tab_drag_data() {
    return &(drag_data_[source_tab_index_]);
  }

  
  TabContentsWrapper* source_dragged_contents() {
    return source_tab_drag_data()->contents;
  }

  
  
  bool AreTabsConsecutive();

  
  TabStripModel* GetModel(BaseTabStrip* tabstrip) const;

  
  NotificationRegistrar registrar_;

  
  BaseTabStrip* source_tabstrip_;

  
  
  BaseTabStrip* attached_tabstrip_;

  
  scoped_ptr<DraggedTabView> view_;

  
  
  
  gfx::Point start_screen_point_;

  
  
  
  
  gfx::Point mouse_offset_;

  
  int source_tab_offset_;

  
  
  float offset_to_width_ratio_;

  
  
  
  
  gfx::Point window_create_point_;

  
  
  gfx::Point first_source_tab_point_;

  
  
  
  
  
  gfx::Rect restore_bounds_;

  
  
  
  views::View* old_focused_view_;

  
  
  int last_move_screen_loc_;

  DockInfo dock_info_;

  DockWindows dock_windows_;

  std::vector<DockDisplayer*> dock_controllers_;

  
  
  
  base::OneShotTimer<DraggedTabController> bring_to_front_timer_;

  
  bool started_drag_;

  
  bool active_;

  DragData drag_data_;

  
  size_t source_tab_index_;

  
  bool initial_move_;

  DISALLOW_COPY_AND_ASSIGN(DraggedTabController);
};

#endif  
