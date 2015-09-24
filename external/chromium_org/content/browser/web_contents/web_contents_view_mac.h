// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_MAC_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_MAC_H_

#import <Cocoa/Cocoa.h>

#include <string>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "content/common/drag_event_source_info.h"
#include "content/port/browser/render_view_host_delegate_view.h"
#include "content/port/browser/web_contents_view_port.h"
#include "ui/base/cocoa/base_view.h"
#include "ui/gfx/size.h"

@class FocusTracker;
class SkBitmap;
@class WebDragDest;
@class WebDragSource;

namespace content {
class WebContentsImpl;
class WebContentsViewDelegate;
class WebContentsViewMac;
}

namespace gfx {
class Vector2d;
}

CONTENT_EXPORT
@interface WebContentsViewCocoa : BaseView {
 @private
  content::WebContentsViewMac* webContentsView_;  
  base::scoped_nsobject<WebDragSource> dragSource_;
  base::scoped_nsobject<WebDragDest> dragDest_;
  BOOL mouseDownCanMoveWindow_;
}

- (void)setMouseDownCanMoveWindow:(BOOL)canMove;

- (content::WebContentsImpl*)webContents;
@end

namespace content {

class WebContentsViewMac : public WebContentsViewPort,
                           public RenderViewHostDelegateView {
 public:
  
  
  
  WebContentsViewMac(WebContentsImpl* web_contents,
                     WebContentsViewDelegate* delegate);
  virtual ~WebContentsViewMac();

  
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
  virtual void SetAllowOverlappingViews(bool overlapping) OVERRIDE;
  virtual bool GetAllowOverlappingViews() const OVERRIDE;
  virtual void SetOverlayView(WebContentsView* overlay,
                              const gfx::Point& offset) OVERRIDE;
  virtual void RemoveOverlayView() OVERRIDE;

  
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
  virtual bool IsEventTracking() const OVERRIDE;
  virtual void CloseTabAfterEventTracking() OVERRIDE;

  
  virtual void ShowContextMenu(const ContextMenuParams& params) OVERRIDE;
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<MenuItem>& items,
                             bool right_aligned,
                             bool allow_multiple_selection) OVERRIDE;
  virtual void StartDragging(const DropData& drop_data,
                             blink::WebDragOperationsMask allowed_operations,
                             const gfx::ImageSkia& image,
                             const gfx::Vector2d& image_offset,
                             const DragEventSourceInfo& event_info) OVERRIDE;
  virtual void UpdateDragCursor(blink::WebDragOperation operation) OVERRIDE;
  virtual void GotFocus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;

  
  
  void CloseTab();

  WebContentsImpl* web_contents() { return web_contents_; }
  WebContentsViewDelegate* delegate() { return delegate_.get(); }

 private:
  
  void UpdateRenderWidgetHostViewOverlay();

  
  WebContentsImpl* web_contents_;

  
  base::scoped_nsobject<WebContentsViewCocoa> cocoa_view_;

  
  
  base::scoped_nsobject<FocusTracker> focus_tracker_;

  
  scoped_ptr<WebContentsViewDelegate> delegate_;

  
  bool allow_overlapping_views_;

  
  
  WebContentsViewMac* overlay_view_;

  
  gfx::Point overlay_view_offset_;

  
  
  WebContentsViewMac* underlay_view_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsViewMac);
};

}  

#endif  
