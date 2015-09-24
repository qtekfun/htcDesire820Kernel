// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_TAB_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_TAB_GTK_H_
#pragma once

#include "base/basictypes.h"
#include "base/message_loop.h"
#include "chrome/browser/tabs/tab_strip_model.h"
#include "chrome/browser/ui/gtk/tabs/tab_renderer_gtk.h"
#include "ui/base/gtk/gtk_signal.h"

namespace gfx {
class Path;
}

namespace ui {
class ThemeProvider;
}

class TabGtk : public TabRendererGtk,
               public MessageLoopForUI::Observer {
 public:
  
  
  
  class TabDelegate {
   public:
    
    virtual bool IsTabSelected(const TabGtk* tab) const = 0;

    
    virtual bool IsTabPinned(const TabGtk* tab) const = 0;

    
    virtual bool IsTabDetached(const TabGtk* tab) const = 0;

    
    virtual void SelectTab(TabGtk* tab) = 0;

    
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

    
    virtual ui::ThemeProvider* GetThemeProvider() = 0;

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

  
  virtual bool IsSelected() const;
  virtual bool IsVisible() const;
  virtual void SetVisible(bool visible) const;
  virtual void CloseButtonClicked();
  virtual void UpdateData(TabContents* contents, bool app, bool loading_only);
  virtual void SetBounds(const gfx::Rect& bounds);

 private:
  class ContextMenuController;
  class TabGtkObserverHelper;
  friend class ContextMenuController;

  
  virtual void WillProcessEvent(GdkEvent* event);
  virtual void DidProcessEvent(GdkEvent* event);

  
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

  
  scoped_ptr<ContextMenuController> menu_controller_;

  
  
  
  
  
  
  GtkWidget* event_box_;

  
  GdkEvent* last_mouse_down_;

  
  
  
  GtkWidget* drag_widget_;

  
  
  int title_width_;

  
  scoped_ptr<TabGtkObserverHelper> observer_;

  
  ScopedRunnableMethodFactory<TabGtk> destroy_factory_;

  
  
  
  
  
  ScopedRunnableMethodFactory<TabGtk> drag_end_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabGtk);
};

#endif  
