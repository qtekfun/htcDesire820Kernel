// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_TAB_CONTENTS_VIEW_GTK_H_
#define CHROME_BROWSER_TAB_CONTENTS_TAB_CONTENTS_VIEW_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/focus_store_gtk.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/browser/tab_contents/tab_contents_view.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class ConstrainedWindowGtk;
class RenderViewContextMenuGtk;
class SadTabGtk;
class TabContentsDragSource;
class WebDragDestGtk;

class TabContentsViewGtk : public TabContentsView,
                           public NotificationObserver {
 public:
  
  
  
  explicit TabContentsViewGtk(TabContents* tab_contents);
  virtual ~TabContentsViewGtk();

  
  
  void AttachConstrainedWindow(ConstrainedWindowGtk* constrained_window);
  void RemoveConstrainedWindow(ConstrainedWindowGtk* constrained_window);

  
  
  void SetFocusedWidget(GtkWidget* widget);

  

  virtual void CreateView(const gfx::Size& initial_size);
  virtual RenderWidgetHostView* CreateViewForWidget(
      RenderWidgetHost* render_widget_host);

  virtual gfx::NativeView GetNativeView() const;
  virtual gfx::NativeView GetContentNativeView() const;
  virtual gfx::NativeWindow GetTopLevelNativeWindow() const;
  virtual void GetContainerBounds(gfx::Rect* out) const;
  virtual void SetPageTitle(const std::wstring& title);
  virtual void OnTabCrashed(base::TerminationStatus status,
                            int error_code);
  virtual void SizeContents(const gfx::Size& size);
  virtual void Focus();
  virtual void SetInitialFocus();
  virtual void StoreFocus();
  virtual void RestoreFocus();
  virtual void GetViewBounds(gfx::Rect* out) const;

  
  virtual void ShowContextMenu(const ContextMenuParams& params);
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned);
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask allowed_ops,
                             const SkBitmap& image,
                             const gfx::Point& image_offset);
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation);
  virtual void GotFocus();
  virtual void TakeFocus(bool reverse);

  

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  
  
  
  void InsertIntoContentArea(GtkWidget* widget);

  void CancelDragIfAny();

  
  CHROMEGTK_CALLBACK_1(TabContentsViewGtk, gboolean, OnFocus, GtkDirectionType);

  
  
  CHROMEGTK_CALLBACK_2(TabContentsViewGtk, void, OnChildSizeRequest,
                       GtkWidget*, GtkRequisition*);

  
  
  CHROMEGTK_CALLBACK_1(TabContentsViewGtk, void, OnSizeAllocate,
                       GtkAllocation*);

  CHROMEGTK_CALLBACK_1(TabContentsViewGtk, void, OnSetFloatingPosition,
                       GtkAllocation*);

  
  OwnedWidgetGtk floating_;

  
  
  GtkWidget* expanded_;

  
  
  scoped_ptr<RenderViewContextMenuGtk> context_menu_;

  
  NotificationRegistrar registrar_;

  scoped_ptr<SadTabGtk> sad_tab_;

  FocusStoreGtk focus_store_;

  
  
  ConstrainedWindowGtk* constrained_window_;

  
  
  scoped_ptr<WebDragDestGtk> drag_dest_;

  
  scoped_ptr<TabContentsDragSource> drag_source_;

  
  
  gfx::Size requested_size_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsViewGtk);
};

#endif  
