// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_TAB_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_TAB_GTK_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "chrome/browser/ui/gtk/tabs/tab_renderer_gtk.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "ui/base/gtk/gtk_signal.h"

class TabStripMenuController;
class ThemeService;

class TabGtk : public TabRendererGtk, public base::MessageLoopForUI::Observer {
 public:
  
  
  
  class TabDelegate {
   public:
    
    virtual bool IsTabActive(const TabGtk* tab) const = 0;

    
    virtual bool IsTabSelected(const TabGtk* tab) const = 0;

    
    virtual bool IsTabPinned(const TabGtk* tab) const = 0;

    
    virtual bool IsTabDetached(const TabGtk* tab) const = 0;

    
    virtual void ActivateTab(TabGtk* tab) = 0;

    
    virtual void ToggleTabSelection(TabGtk* tab) = 0;

    
    virtual void ExtendTabSelection(TabGtk* tab) = 0;

    
    virtual void CloseTab(TabGtk* tab) = 0;

    
    virtual bool IsCommandEnabledForTab(
        TabStripModel::ContextMenuCommand command_id,
        const TabGtk* tab) const = 0;

    
    virtual void ExecuteCommandForTab(
        TabStripModel::ContextMenuCommand command_id, TabGtk* tab) = 0;

    
    
    virtual void StartHighlightTabsForCommand(
        TabStripModel::ContextMenuCommand command_id, TabGtk* tab) = 0;
    virtual void StopHighlightTabsForCommand(
        TabStripModel::ContextMenuCommand command_id, TabGtk* tab) = 0;
    virtual void StopAllHighlighting() = 0;

    
    virtual void MaybeStartDrag(TabGtk* tab, const gfx::Point& point) = 0;

    
    virtual void ContinueDrag(GdkDragContext* context) = 0;

    
    
    
    virtual bool EndDrag(bool canceled) = 0;

    
    
    
    
    virtual bool HasAvailableDragActions() const = 0;

    
    virtual GtkThemeService* GetThemeProvider() = 0;

    
    
    virtual TabStripMenuController* GetTabStripMenuControllerForTab(
        TabGtk* tab) = 0;

   protected:
    virtual ~TabDelegate() {}
  };

  explicit TabGtk(TabDelegate* delegate);
  virtual ~TabGtk();

  
  TabDelegate* delegate() const { return delegate_; }

  GtkWidget* widget() const { return event_box_; }

  
  void set_closing(bool closing) { closing_ = closing; }
  bool closing() const { return closing_; }

  
  void set_dragging(bool dragging) { dragging_ = dragging; }
  bool dragging() const { return dragging_; }

  
  void Raise() const;

  
  virtual bool IsActive() const OVERRIDE;
  virtual bool IsSelected() const OVERRIDE;
  virtual bool IsVisible() const OVERRIDE;
  virtual void SetVisible(bool visible) const OVERRIDE;
  virtual void CloseButtonClicked() OVERRIDE;
  virtual void UpdateData(content::WebContents* contents,
                          bool app,
                          bool loading_only) OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;

 private:
  class TabGtkObserverHelper;

  
  virtual void WillProcessEvent(GdkEvent* event) OVERRIDE;
  virtual void DidProcessEvent(GdkEvent* event) OVERRIDE;

  
  CHROMEGTK_CALLBACK_1(TabGtk, gboolean, OnButtonPressEvent, GdkEventButton*);

  
  CHROMEGTK_CALLBACK_1(TabGtk, gboolean, OnButtonReleaseEvent, GdkEventButton*);

  
  
  CHROMEGTK_CALLBACK_1(TabGtk, void, OnDragBegin, GdkDragContext*);

  
  
  
  CHROMEGTK_CALLBACK_2(TabGtk, gboolean, OnDragFailed, GdkDragContext*,
                       GtkDragResult);

  
  
  
  
  CHROMEGTK_CALLBACK_1(TabGtk, gboolean, OnDragButtonReleased, GdkEventButton*);

  
  void ContextMenuClosed();

  
  
  void UpdateTooltipState();

  
  void CreateDragWidget();

  
  void DestroyDragWidget();

  
  
  void StartDragging(gfx::Point drag_offset);

  
  
  void EndDrag(bool canceled);

  
  
  TabDelegate* delegate_;

  
  bool closing_;

  
  bool dragging_;

  
  scoped_ptr<TabStripMenuController> menu_controller_;

  
  
  
  
  
  
  GtkWidget* event_box_;

  
  GdkEvent* last_mouse_down_;

  
  
  
  GtkWidget* drag_widget_;

  
  
  int title_width_;

  
  scoped_ptr<TabGtkObserverHelper> observer_;

  
  base::WeakPtrFactory<TabGtk> destroy_factory_;

  
  
  
  
  
  base::WeakPtrFactory<TabGtk> drag_end_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabGtk);
};

#endif  
