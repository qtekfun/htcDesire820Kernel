// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_VIEW_GTK_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_VIEW_GTK_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/browser/tab_contents/tab_contents_view.h"
#include "ui/gfx/size.h"
#include "views/widget/widget_gtk.h"

class ConstrainedWindowGtk;
typedef struct _GtkFloatingContainer GtkFloatingContainer;
class RenderViewContextMenuViews;
class SadTabView;
class SkBitmap;
class TabContentsDragSource;
class WebDragDestGtk;
namespace gfx {
class Point;
}
namespace views {
class NativeViewHost;
}

class TabContentsViewGtk : public TabContentsView,
                           public views::WidgetGtk {
 public:
  
  
  
  explicit TabContentsViewGtk(TabContents* tab_contents);
  virtual ~TabContentsViewGtk();

  
  
  void AttachConstrainedWindow(ConstrainedWindowGtk* constrained_window);
  void RemoveConstrainedWindow(ConstrainedWindowGtk* constrained_window);

  gboolean OnMouseMove(GtkWidget* widget, GdkEventMotion* event);

  

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
                             WebKit::WebDragOperationsMask ops_allowed,
                             const SkBitmap& image,
                             const gfx::Point& image_offset);
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation);
  virtual void GotFocus();
  virtual void TakeFocus(bool reverse);

 private:
  

  
  virtual gboolean OnButtonPress(GtkWidget* widget, GdkEventButton* event);
  virtual void OnSizeAllocate(GtkWidget* widget, GtkAllocation* allocation);
  virtual gboolean OnPaint(GtkWidget* widget, GdkEventExpose* event);
  virtual void OnShow(GtkWidget* widget);
  virtual void OnHide(GtkWidget* widget);

  
  
  void WasHidden();
  void WasShown();

  
  
  void WasSized(const gfx::Size& size);

  
  
  
  void SetFloatingPosition(const gfx::Size& size);

  

  
  
  SadTabView* sad_tab_;

  
  bool ignore_next_char_event_;

  
  int last_focused_view_storage_id_;

  
  scoped_ptr<RenderViewContextMenuViews> context_menu_;

  
  scoped_ptr<TabContentsDragSource> drag_source_;

  
  GdkEventButton last_mouse_down_;

  
  
  scoped_ptr<WebDragDestGtk> drag_dest_;

  
  gfx::Size size_;

  
  
  std::vector<ConstrainedWindowGtk*> constrained_windows_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsViewGtk);
};

#endif  
