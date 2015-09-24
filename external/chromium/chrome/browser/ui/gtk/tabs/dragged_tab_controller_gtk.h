// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_DRAGGED_TAB_CONTROLLER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_DRAGGED_TAB_CONTROLLER_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <set>

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/ui/tabs/dock_info.h"
#include "content/browser/tab_contents/tab_contents_delegate.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/x/x11_util.h"

class DraggedTabGtk;
class TabGtk;
class TabStripGtk;
class TabContentsWrapper;

class DraggedTabControllerGtk : public NotificationObserver,
                                public TabContentsDelegate {
 public:
  DraggedTabControllerGtk(TabGtk* source_tab, TabStripGtk* source_tabstrip);
  virtual ~DraggedTabControllerGtk();

  
  
  
  void CaptureDragInfo(const gfx::Point& mouse_offset);

  
  
  
  void Drag();

  
  
  
  
  bool EndDrag(bool canceled);

  
  
  
  TabGtk* GetDragSourceTabForContents(TabContents* contents) const;

  
  bool IsDragSourceTab(const TabGtk* tab) const;

  
  bool IsTabDetached(const TabGtk* tab) const;

 private:
  
  enum EndDragType {
    
    NORMAL,

    
    CANCELED,

    
    TAB_DESTROYED
  };

  
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition);
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags);
  virtual void AddNewContents(TabContents* source,
                              TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture);
  virtual void ActivateContents(TabContents* contents);
  virtual void DeactivateContents(TabContents* contents);
  virtual void LoadingStateChanged(TabContents* source);
  virtual void CloseContents(TabContents* source);
  virtual void MoveContents(TabContents* source, const gfx::Rect& pos);
  virtual bool IsPopup(const TabContents* source) const;
  virtual void UpdateTargetURL(TabContents* source, const GURL& url);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void InitWindowCreatePoint();

  
  
  gfx::Point GetWindowCreatePoint() const;

  
  void SetDraggedContents(TabContentsWrapper* new_contents);

  
  
  void ContinueDragging();

  
  void MoveTab(const gfx::Point& screen_point);

  
  
  TabStripGtk* GetTabStripForPoint(const gfx::Point& screen_point);

  
  
  TabStripGtk* GetTabStripIfItContains(TabStripGtk* tabstrip,
                                       const gfx::Point& screen_point) const;

  
  void Attach(TabStripGtk* attached_tabstrip, const gfx::Point& screen_point);

  
  void Detach();

  
  gfx::Point ConvertScreenPointToTabStripPoint(TabStripGtk* tabstrip,
                                               const gfx::Point& screen_point);

  
  
  gfx::Rect GetDraggedTabTabStripBounds(const gfx::Point& screen_point);

  
  
  
  
  int GetInsertionIndexForDraggedBounds(const gfx::Rect& dragged_bounds,
                                        bool is_tab_attached) const;

  
  gfx::Point GetDraggedTabPoint(const gfx::Point& screen_point);

  
  
  TabGtk* GetTabMatchingDraggedContents(TabStripGtk* tabstrip) const;

  
  bool EndDragImpl(EndDragType how_end);

  
  
  void RevertDrag();

  
  
  bool CompleteDrag();

  
  void EnsureDraggedTab();

  
  gfx::Point GetCursorScreenPoint() const;

  
  static gfx::Rect GetTabScreenBounds(TabGtk* tab);

  
  
  int NormalizeIndexToAttachedTabStrip(int index) const;

  
  
  void HideWindow();

  
  void ShowWindow();

  
  void CleanUpHiddenFrame();

  
  void CleanUpSourceTab();

  
  
  void OnAnimateToBoundsComplete();

  
  void BringWindowUnderMouseToFront();

  
  NotificationRegistrar registrar_;

  
  TabContentsWrapper* dragged_contents_;

  
  
  
  TabContentsDelegate* original_delegate_;

  
  TabGtk* source_tab_;

  
  TabStripGtk* source_tabstrip_;

  
  
  int source_model_index_;

  
  
  TabStripGtk* attached_tabstrip_;

  
  scoped_ptr<DraggedTabGtk> dragged_tab_;

  
  
  
  gfx::Point start_screen_point_;

  
  
  
  
  gfx::Point mouse_offset_;

  
  
  
  
  gfx::Point window_create_point_;

  
  
  bool in_destructor_;

  
  
  int last_move_screen_x_;

  
  DockInfo dock_info_;

  typedef std::set<GtkWidget*> DockWindows;
  DockWindows dock_windows_;

  
  const bool mini_;

  
  const bool pinned_;

  
  
  
  base::OneShotTimer<DraggedTabControllerGtk> bring_to_front_timer_;

  DISALLOW_COPY_AND_ASSIGN(DraggedTabControllerGtk);
};

#endif  
