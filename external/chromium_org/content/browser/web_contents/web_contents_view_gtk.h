// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_GTK_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_GTK_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "content/common/drag_event_source_info.h"
#include "content/port/browser/render_view_host_delegate_view.h"
#include "content/port/browser/web_contents_view_port.h"
#include "ui/base/gtk/focus_store_gtk.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"

namespace content {

class WebContents;
class WebContentsImpl;
class WebContentsViewDelegate;
class WebDragDestDelegate;
class WebDragDestGtk;
class WebDragSourceGtk;

class CONTENT_EXPORT WebContentsViewGtk
    : public WebContentsViewPort,
      public RenderViewHostDelegateView {
 public:
  
  
  
  
  
  WebContentsViewGtk(WebContentsImpl* web_contents,
                     WebContentsViewDelegate* delegate);
  virtual ~WebContentsViewGtk();

  WebContentsViewDelegate* delegate() const { return delegate_.get(); }
  WebContents* web_contents();

  

  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeView GetContentNativeView() const OVERRIDE;
  virtual gfx::NativeWindow GetTopLevelNativeWindow() const OVERRIDE;
  virtual void GetContainerBounds(gfx::Rect* out) const OVERRIDE;
  virtual void OnTabCrashed(base::TerminationStatus status,
                            int error_code) OVERRIDE;
  virtual void SizeContents(const gfx::Size& size) OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void SetInitialFocus() OVERRIDE;
  virtual void StoreFocus() OVERRIDE;
  virtual void RestoreFocus() OVERRIDE;
  virtual DropData* GetDropData() const OVERRIDE;
  virtual gfx::Rect GetViewBounds() const OVERRIDE;

  
  virtual void CreateView(
      const gfx::Size& initial_size, gfx::NativeView context) OVERRIDE;
  virtual RenderWidgetHostView* CreateViewForWidget(
      RenderWidgetHost* render_widget_host) OVERRIDE;
  virtual RenderWidgetHostView* CreateViewForPopupWidget(
      RenderWidgetHost* render_widget_host) OVERRIDE;
  virtual void SetPageTitle(const base::string16& title) OVERRIDE;
  virtual void RenderViewCreated(RenderViewHost* host) OVERRIDE;
  virtual void RenderViewSwappedIn(RenderViewHost* host) OVERRIDE;
  virtual void SetOverscrollControllerEnabled(bool enabled) OVERRIDE;

  
  virtual void ShowContextMenu(const ContextMenuParams& params) OVERRIDE;
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<MenuItem>& items,
                             bool right_aligned,
                             bool allow_multiple_selection) OVERRIDE;
  virtual void StartDragging(const DropData& drop_data,
                             blink::WebDragOperationsMask allowed_ops,
                             const gfx::ImageSkia& image,
                             const gfx::Vector2d& image_offset,
                             const DragEventSourceInfo& event_info) OVERRIDE;
  virtual void UpdateDragCursor(blink::WebDragOperation operation) OVERRIDE;
  virtual void GotFocus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;

 private:
  
  
  
  
  void InsertIntoContentArea(GtkWidget* widget);

  
  
  void UpdateDragDest(RenderViewHost* new_host);

  
  
  CHROMEGTK_CALLBACK_1(WebContentsViewGtk, gboolean, OnFocus, GtkDirectionType);

  
  
  CHROMEGTK_CALLBACK_2(WebContentsViewGtk, void, OnChildSizeRequest,
                       GtkWidget*, GtkRequisition*);

  
  
  CHROMEGTK_CALLBACK_1(WebContentsViewGtk, void, OnSizeAllocate,
                       GtkAllocation*);

  
  WebContentsImpl* web_contents_;

  
  
  ui::OwnedWidgetGtk expanded_;

  ui::FocusStoreGtk focus_store_;

  
  
  scoped_ptr<WebDragDestGtk> drag_dest_;

  
  scoped_ptr<WebDragSourceGtk> drag_source_;

  
  
  
  scoped_ptr<WebContentsViewDelegate> delegate_;

  
  
  gfx::Size requested_size_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsViewGtk);
};

}  

#endif  
