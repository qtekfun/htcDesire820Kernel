// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_TAB_CONTENTS_VIEW_MAC_H_
#define CHROME_BROWSER_TAB_CONTENTS_TAB_CONTENTS_VIEW_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include <string>
#include <vector>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/base_view.h"
#include "content/browser/tab_contents/tab_contents_view.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/size.h"

@class FocusTracker;
@class SadTabController;
class SkBitmap;
class TabContentsViewMac;
@class WebDragSource;
@class WebDropTarget;
namespace gfx {
class Point;
}

@interface TabContentsViewCocoa : BaseView {
 @private
  TabContentsViewMac* tabContentsView_;  
  scoped_nsobject<WebDragSource> dragSource_;
  scoped_nsobject<WebDropTarget> dropTarget_;
}

- (TabContents*)tabContents;
@end

class TabContentsViewMac : public TabContentsView,
                           public NotificationObserver {
 public:
  
  
  
  explicit TabContentsViewMac(TabContents* web_contents);
  virtual ~TabContentsViewMac();

  

  virtual void CreateView(const gfx::Size& initial_size);
  virtual RenderWidgetHostView* CreateViewForWidget(
      RenderWidgetHost* render_widget_host);
  virtual gfx::NativeView GetNativeView() const;
  virtual gfx::NativeView GetContentNativeView() const;
  virtual gfx::NativeWindow GetTopLevelNativeWindow() const;
  virtual void GetContainerBounds(gfx::Rect* out) const;
  virtual void RenderViewCreated(RenderViewHost* host);
  virtual void SetPageTitle(const std::wstring& title);
  virtual void OnTabCrashed(base::TerminationStatus status,
                            int error_code);
  virtual void SizeContents(const gfx::Size& size);
  virtual void Focus();
  virtual void SetInitialFocus();
  virtual void StoreFocus();
  virtual void RestoreFocus();
  virtual void UpdatePreferredSize(const gfx::Size& pref_size);
  virtual RenderWidgetHostView* CreateNewWidgetInternal(
      int route_id,
      WebKit::WebPopupType popup_type);
  virtual void ShowCreatedWidgetInternal(RenderWidgetHostView* widget_host_view,
                                         const gfx::Rect& initial_pos);
  virtual bool IsEventTracking() const;
  virtual void CloseTabAfterEventTracking();
  virtual void GetViewBounds(gfx::Rect* out) const;

  
  virtual void ShowContextMenu(const ContextMenuParams& params);
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned);
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask allowed_operations,
                             const SkBitmap& image,
                             const gfx::Point& image_offset);
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation);
  virtual void GotFocus();
  virtual void TakeFocus(bool reverse);

  

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void CloseTab();

  int preferred_width() const { return preferred_width_; }

 private:
  
  scoped_nsobject<TabContentsViewCocoa> cocoa_view_;

  
  
  scoped_nsobject<FocusTracker> focus_tracker_;

  
  NotificationRegistrar registrar_;

  
  
  scoped_nsobject<SadTabController> sad_tab_;

  
  int preferred_width_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsViewMac);
};

#endif  
