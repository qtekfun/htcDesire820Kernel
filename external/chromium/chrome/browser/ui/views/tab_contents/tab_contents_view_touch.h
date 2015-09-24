// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_VIEW_TOUCH_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_VIEW_TOUCH_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/browser/tab_contents/tab_contents_view.h"
#include "ui/gfx/size.h"
#include "views/view.h"

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

class TabContentsViewTouch : public TabContentsView, public views::View {
 public:
  
  
  
  explicit TabContentsViewTouch(TabContents* tab_contents);
  virtual ~TabContentsViewTouch();

  
  
  void AttachConstrainedWindow(ConstrainedWindowGtk* constrained_window);
  void RemoveConstrainedWindow(ConstrainedWindowGtk* constrained_window);

  
  virtual void CreateView(const gfx::Size& initial_size) OVERRIDE;
  virtual RenderWidgetHostView* CreateViewForWidget(
      RenderWidgetHost* render_widget_host) OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeView GetContentNativeView() const OVERRIDE;
  virtual gfx::NativeWindow GetTopLevelNativeWindow() const OVERRIDE;
  virtual void GetContainerBounds(gfx::Rect* out) const OVERRIDE;
  virtual void SetPageTitle(const std::wstring& title) OVERRIDE;
  virtual void OnTabCrashed(base::TerminationStatus status,
                            int error_code) OVERRIDE;
  virtual void SizeContents(const gfx::Size& size) OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void SetInitialFocus() OVERRIDE;
  virtual void StoreFocus() OVERRIDE;
  virtual void RestoreFocus() OVERRIDE;
  virtual void GetViewBounds(gfx::Rect* out) const OVERRIDE;

  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void ShowContextMenu(const ContextMenuParams& params);
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned) OVERRIDE;
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask ops_allowed,
                             const SkBitmap& image,
                             const gfx::Point& image_offset) OVERRIDE;
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation) OVERRIDE;
  virtual void GotFocus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;
  virtual void VisibilityChanged(views::View *, bool is_visible) OVERRIDE;

 private:
  

  
  
  void WasHidden();
  void WasShown();

  
  
  void WasSized(const gfx::Size& size);

  
  
  
  void SetFloatingPosition(const gfx::Size& size);

  
  
  scoped_ptr<SadTabView> sad_tab_;

  
  bool ignore_next_char_event_;

  
  int last_focused_view_storage_id_;

  
  scoped_ptr<RenderViewContextMenuViews> context_menu_;

  
  

  
  GdkEventButton last_mouse_down_;

  
  gfx::Size size_;

  
  
  std::vector<ConstrainedWindowGtk*> constrained_windows_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsViewTouch);
};

#endif  
