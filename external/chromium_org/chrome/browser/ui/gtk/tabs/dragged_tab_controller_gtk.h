// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_DRAGGED_TAB_CONTROLLER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_DRAGGED_TAB_CONTROLLER_GTK_H_

#include <gtk/gtk.h>

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/gtk/tabs/drag_data.h"
#include "chrome/browser/ui/tabs/dock_info.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/base/x/x11_util.h"

class DraggedViewGtk;
class TabGtk;
class TabStripGtk;
class TabStripModel;

class DraggedTabControllerGtk : public content::NotificationObserver,
                                public content::WebContentsDelegate {
 public:
  
  
  
  
  DraggedTabControllerGtk(TabStripGtk* source_tabstrip, TabGtk* source_tab,
                          const std::vector<TabGtk*>& tabs);
  virtual ~DraggedTabControllerGtk();

  
  
  
  void CaptureDragInfo(const gfx::Point& mouse_offset);

  
  
  
  void Drag();

  
  
  
  
  bool EndDrag(bool canceled);

  
  
  
  TabGtk* GetDraggedTabForContents(content::WebContents* contents);

  
  bool IsDraggingTab(const TabGtk* tab);

  
  bool IsDraggingWebContents(const content::WebContents* web_contents);

  
  bool IsTabDetached(const TabGtk* tab);

 private:
  
  enum EndDragType {
    
    NORMAL,

    
    CANCELED,

    
    TAB_DESTROYED
  };

  DraggedTabData InitDraggedTabData(TabGtk* tab);

  
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
  virtual content::JavaScriptDialogManager*
      GetJavaScriptDialogManager() OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  gfx::Point GetWindowCreatePoint() const;

  
  
  void ContinueDragging();

  
  void MoveAttached(const gfx::Point& screen_point);

  
  void MoveDetached(const gfx::Point& screen_point);

  
  void RestoreSelection(TabStripModel* model);

  
  
  TabStripGtk* GetTabStripForPoint(const gfx::Point& screen_point);

  
  
  TabStripGtk* GetTabStripIfItContains(TabStripGtk* tabstrip,
                                       const gfx::Point& screen_point) const;

  
  void Attach(TabStripGtk* attached_tabstrip, const gfx::Point& screen_point);

  
  void Detach();

  
  gfx::Point ConvertScreenPointToTabStripPoint(TabStripGtk* tabstrip,
                                               const gfx::Point& screen_point);

  
  
  gfx::Rect GetDraggedViewTabStripBounds(const gfx::Point& screen_point);

  
  
  
  int GetInsertionIndexForDraggedBounds(const gfx::Rect& dragged_bounds);

  
  
  gfx::Point GetDraggedViewPoint(const gfx::Point& screen_point);

  
  
  TabGtk* GetTabMatchingDraggedContents(TabStripGtk* tabstrip,
                                        content::WebContents* contents);

  
  
  std::vector<TabGtk*> GetTabsMatchingDraggedContents(TabStripGtk* tabstrip);

  
  
  void SetDraggedTabsVisible(bool visible, bool repaint);

  
  bool EndDragImpl(EndDragType how_end);

  
  
  void RevertDrag();

  
  
  bool CompleteDrag();

  
  void ResetDelegates();

  
  void EnsureDraggedView();

  
  gfx::Rect GetAnimateBounds();

  
  
  int NormalizeIndexToAttachedTabStrip(int index) const;

  
  
  void HideWindow();

  
  void ShowWindow();

  
  void CleanUpHiddenFrame();

  
  void CleanUpDraggedTabs();

  
  
  void OnAnimateToBoundsComplete();

  
  void BringWindowUnderMouseToFront();

  
  
  bool AreTabsConsecutive();

  
  content::NotificationRegistrar registrar_;

  
  TabStripGtk* source_tabstrip_;

  
  scoped_ptr<DragData> drag_data_;

  
  
  TabStripGtk* attached_tabstrip_;

  
  scoped_ptr<DraggedViewGtk> dragged_view_;

  
  
  
  gfx::Point start_screen_point_;

  
  
  
  
  gfx::Point mouse_offset_;

  
  
  bool in_destructor_;

  
  
  int last_move_screen_x_;

  
  bool initial_move_;

  
  DockInfo dock_info_;

  typedef std::set<GtkWidget*> DockWindows;
  DockWindows dock_windows_;

  
  
  
  base::OneShotTimer<DraggedTabControllerGtk> bring_to_front_timer_;

  DISALLOW_COPY_AND_ASSIGN(DraggedTabControllerGtk);
};

#endif  
