// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_TAB_STRIP_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_TAB_STRIP_GTK_H_

#include <gtk/gtk.h>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "chrome/browser/ui/gtk/tabs/tab_gtk.h"
#include "chrome/browser/ui/gtk/tabstrip_origin_provider.h"
#include "chrome/browser/ui/gtk/view_id_util.h"
#include "chrome/browser/ui/tabs/hover_tab_selector.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/rect.h"

class BrowserWindowGtk;
class CustomDrawButton;
class DraggedTabControllerGtk;
class GtkThemeService;

namespace gfx {
class Image;
}

class TabStripGtk : public TabStripModelObserver,
                    public TabGtk::TabDelegate,
                    public base::MessageLoopForUI::Observer,
                    public content::NotificationObserver,
                    public TabstripOriginProvider,
                    public ViewIDUtil::Delegate {
 public:
  class TabAnimation;

  TabStripGtk(TabStripModel* model, BrowserWindowGtk* window);
  virtual ~TabStripGtk();

  
  
  void Init();

  void Show();
  void Hide();

  TabStripModel* model() const { return model_; }

  BrowserWindowGtk* window() const { return window_; }

  GtkWidget* widget() const { return tabstrip_.get(); }

  
  bool IsDragSessionActive() const { return drag_controller_.get() != NULL; }

  
  bool IsActiveDropTarget() const;

  
  void Layout();

  
  void SchedulePaint();

  
  void SetBounds(const gfx::Rect& bounds);

  
  const gfx::Rect& bounds() const { return bounds_; }

  
  void UpdateLoadingAnimations();

  
  
  bool IsCompatibleWith(TabStripGtk* other);

  
  
  bool IsAnimating() const;

  
  void DestroyDragController();

  
  void DestroyDraggedTab(TabGtk* dragged_tab);

  
  gfx::Rect GetIdealBounds(int index);

  
  
  
  void SetVerticalOffset(int offset);

  
  virtual gfx::Point GetTabStripOriginForWidget(GtkWidget* widget) OVERRIDE;

  
  virtual GtkWidget* GetWidgetForViewID(ViewID id) OVERRIDE;

 protected:
  
  virtual void TabInsertedAt(content::WebContents* contents,
                             int index,
                             bool foreground) OVERRIDE;
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void TabMoved(content::WebContents* contents,
                        int from_index,
                        int to_index) OVERRIDE;
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;
  virtual void TabSelectionChanged(
      TabStripModel* tab_strip_model,
      const ui::ListSelectionModel& old_model) OVERRIDE;
  virtual void TabChangedAt(content::WebContents* contents,
                            int index,
                            TabChangeType change_type) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabMiniStateChanged(content::WebContents* contents,
                                   int index) OVERRIDE;
  virtual void TabBlockedStateChanged(content::WebContents* contents,
                                      int index) OVERRIDE;

  
  virtual bool IsTabActive(const TabGtk* tab) const OVERRIDE;
  virtual bool IsTabSelected(const TabGtk* tab) const OVERRIDE;
  virtual bool IsTabPinned(const TabGtk* tab) const OVERRIDE;
  virtual bool IsTabDetached(const TabGtk* tab) const OVERRIDE;
  virtual void ActivateTab(TabGtk* tab) OVERRIDE;
  virtual void ToggleTabSelection(TabGtk* tab) OVERRIDE;
  virtual void ExtendTabSelection(TabGtk* tab) OVERRIDE;
  virtual void CloseTab(TabGtk* tab) OVERRIDE;
  virtual bool IsCommandEnabledForTab(
      TabStripModel::ContextMenuCommand command_id,
      const TabGtk* tab) const OVERRIDE;
  virtual void ExecuteCommandForTab(
      TabStripModel::ContextMenuCommand command_id, TabGtk* tab) OVERRIDE;
  virtual void StartHighlightTabsForCommand(
      TabStripModel::ContextMenuCommand command_id, TabGtk* tab) OVERRIDE;
  virtual void StopHighlightTabsForCommand(
      TabStripModel::ContextMenuCommand command_id, TabGtk* tab) OVERRIDE;
  virtual void StopAllHighlighting() OVERRIDE;
  virtual void MaybeStartDrag(TabGtk* tab, const gfx::Point& point) OVERRIDE;
  virtual void ContinueDrag(GdkDragContext* context) OVERRIDE;
  virtual bool EndDrag(bool canceled) OVERRIDE;
  virtual bool HasAvailableDragActions() const OVERRIDE;
  virtual GtkThemeService* GetThemeProvider() OVERRIDE;
  virtual TabStripMenuController* GetTabStripMenuControllerForTab(
      TabGtk* tab) OVERRIDE;

  
  virtual void WillProcessEvent(GdkEvent* event) OVERRIDE;
  virtual void DidProcessEvent(GdkEvent* event) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  static const int mini_to_non_mini_gap_;

 private:
  friend class BrowserWindowGtk;
  friend class DraggedTabControllerGtk;
  friend class InsertTabAnimation;
  friend class MiniMoveAnimation;
  friend class MiniTabAnimation;
  friend class MoveTabAnimation;
  friend class RemoveTabAnimation;
  friend class ResizeLayoutAnimation;
  friend class TabAnimation;

  struct TabData {
    TabGtk* tab;
    gfx::Rect ideal_bounds;
  };

  
  
  class DropInfo {
   public:
    DropInfo(int index, bool drop_before, bool point_down);
    virtual ~DropInfo();

    

    
    CHROMEGTK_CALLBACK_1(DropInfo, gboolean, OnExposeEvent, GdkEventExpose*);

    
    
    void SetContainerColorMap();

    
    
    void SetContainerTransparency();

    
    
    
    void SetContainerShapeMask();

    
    void CreateContainer();

    
    void DestroyContainer();

    
    
    
    
    int drop_index;
    bool drop_before;

    
    
    
    bool point_down;

    
    
    GtkWidget* container;

    
    gfx::Image* drop_arrow;

   private:
    DISALLOW_COPY_AND_ASSIGN(DropInfo);
  };

  
  
  
  CHROMEGTK_CALLBACK_0(TabStripGtk, void, OnMap);

  
  CHROMEGTK_CALLBACK_1(TabStripGtk, gboolean, OnExpose, GdkEventExpose*);

  
  CHROMEGTK_CALLBACK_1(TabStripGtk, void, OnSizeAllocate, GtkAllocation*);

  
  
  CHROMEGTK_CALLBACK_4(TabStripGtk, gboolean, OnDragMotion, GdkDragContext*,
                       gint, gint, guint);

  
  CHROMEGTK_CALLBACK_4(TabStripGtk, gboolean, OnDragDrop, GdkDragContext*,
                       gint, gint, guint);

  
  
  CHROMEGTK_CALLBACK_2(TabStripGtk, gboolean, OnDragLeave, GdkDragContext*,
                       guint);

  
  CHROMEGTK_CALLBACK_6(TabStripGtk, gboolean, OnDragDataReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);

  
  CHROMEGTK_CALLBACK_0(TabStripGtk, void, OnNewTabClicked);

  
  void SetTabBounds(TabGtk* tab, const gfx::Rect& bounds);

  
  
  
  bool CanPaintOnlyFavicons(const GdkRectangle* rects,
                            int num_rects,
                            std::vector<int>* tabs_to_paint);

  
  void PaintOnlyFavicons(GdkEventExpose* event,
                         const std::vector<int>& tabs_to_paint);

  
  CustomDrawButton* MakeNewTabButton();

  
  void SetNewTabButtonBackground();

  
  int GetTabCount() const;

  
  int GetMiniTabCount() const;

  
  
  TabGtk* GetTabAt(int index) const;

  
  
  
  
  
  
  
  TabGtk* GetTabAtAdjustForAnimation(int index) const;

  
  void GetCurrentTabWidths(double* unselected_width,
                           double* selected_width) const;

  
  
  
  
  
  
  void GetDesiredTabWidths(int tab_count,
                           int mini_tab_count,
                           double* unselected_width,
                           double* selected_width) const;

  
  int GetTabHOffset(int tab_index);

  
  int tab_start_x() const;

  
  void ResizeLayoutTabs();

  
  
  bool IsCursorInTabStripZone() const;

  
  void ReStack();

  
  
  void AddMessageLoopObserver();
  void RemoveMessageLoopObserver();

  
  int GetAvailableWidthForTabs(TabGtk* last_tab) const;

  
  
  
  int GetIndexOfTab(const TabGtk* tab) const;

  
  void RemoveTabAt(int index);

  
  
  void HandleGlobalMouseMoveEvent();

  
  
  
  
  void GenerateIdealBounds();

  
  
  
  
  void LayoutNewTabButton(double last_tab_right, double unselected_width);

  

  
  
  
  gfx::Rect GetDropBounds(int drop_index, bool drop_before, bool* is_beneath);

  
  void UpdateDropIndex(GdkDragContext* context, gint x, gint y);

  
  void SetDropIndex(int index, bool drop_before);

  
  
  
  bool CompleteDrop(const guchar* data, bool is_plain_text);

  
  
  static gfx::Image* GetDropArrowImage(bool is_down);

  

  
  void StopAnimation();

  
  
  void AnimationLayout(double unselected_width);

  
  void StartInsertTabAnimation(int index);
  void StartRemoveTabAnimation(int index, content::WebContents* contents);
  void StartMoveTabAnimation(int from_index, int to_index);
  void StartMiniTabAnimation(int index);
  void StartMiniMoveTabAnimation(int from_index,
                                 int to_index,
                                 const gfx::Rect& start_bounds);
  void StartResizeLayoutAnimation();

  
  
  void FinishAnimation(TabAnimation* animation, bool layout);

  
  std::vector<TabData> tab_data_;

  
  
  
  
  double current_unselected_width_;
  double current_selected_width_;

  
  
  
  
  
  
  
  int available_width_for_tabs_;

  
  
  bool needs_resize_layout_;

  
  ui::OwnedWidgetGtk tabstrip_;

  
  gfx::Rect bounds_;

  
  
  int tab_vertical_offset_;

  
  TabStripModel* model_;

  
  BrowserWindowGtk* window_;

  
  GtkThemeService* theme_service_;

  
  scoped_ptr<TabAnimation> active_animation_;

  
  scoped_ptr<CustomDrawButton> newtab_button_;

  
  gfx::Rect newtab_surface_bounds_;

  
  scoped_ptr<DropInfo> drop_info_;

  
  
  scoped_ptr<DraggedTabControllerGtk> drag_controller_;

  
  bool added_as_message_loop_observer_;

  
  HoverTabSelector hover_tab_selector_;

  content::NotificationRegistrar registrar_;

  
  
  base::WeakPtrFactory<TabStripGtk> weak_factory_;

  
  base::WeakPtrFactory<TabStripGtk> layout_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabStripGtk);
};

#endif  
