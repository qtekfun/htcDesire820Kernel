// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_VIEW_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_VIEW_VIEWS_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/ui/views/tab_contents/native_tab_contents_view_delegate.h"
#include "content/browser/tab_contents/tab_contents_view.h"

class NativeTabContentsView;
class RenderViewContextMenuViews;
class SadTabView;
class SkBitmap;
struct WebDropData;
namespace gfx {
class Point;
class Size;
}
namespace views {
class Widget;
}

class TabContentsViewViews : public TabContentsView,
                             public internal::NativeTabContentsViewDelegate {
 public:
  
  
  
  explicit TabContentsViewViews(TabContents* tab_contents);
  virtual ~TabContentsViewViews();

  
  
  virtual void Unparent();

  
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
  virtual bool IsDoingDrag() const OVERRIDE;
  virtual void CancelDragAndCloseTab() OVERRIDE;
  virtual void GetViewBounds(gfx::Rect* out) const OVERRIDE;
  virtual void ShowContextMenu(const ContextMenuParams& params) OVERRIDE;
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned) OVERRIDE;
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask operations,
                             const SkBitmap& image,
                             const gfx::Point& image_offset) OVERRIDE;
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation) OVERRIDE;
  virtual void GotFocus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;

 private:
  
  virtual TabContents* GetTabContents() OVERRIDE;
  virtual bool IsShowingSadTab() const OVERRIDE;
  virtual void OnNativeTabContentsViewShown() OVERRIDE;
  virtual void OnNativeTabContentsViewHidden() OVERRIDE;
  virtual void OnNativeTabContentsViewSized(const gfx::Size& size) OVERRIDE;
  virtual void OnNativeTabContentsViewWheelZoom(int distance) OVERRIDE;
  virtual void OnNativeTabContentsViewMouseDown() OVERRIDE;
  virtual void OnNativeTabContentsViewMouseMove() OVERRIDE;
  virtual void OnNativeTabContentsViewDraggingEnded() OVERRIDE;

  views::Widget* GetWidget();
  const views::Widget* GetWidget() const;

  
  void CloseTab();

  

  
  
  void WasHidden();
  void WasShown();

  
  
  void WasSized(const gfx::Size& size);

  
  void WheelZoom(int distance);

  

  scoped_ptr<NativeTabContentsView> native_tab_contents_view_;

  
  
  SadTabView* sad_tab_;

  
  int last_focused_view_storage_id_;

  
  scoped_ptr<RenderViewContextMenuViews> context_menu_;

  
  
  bool close_tab_after_drag_ends_;

  
  base::OneShotTimer<TabContentsViewViews> close_tab_timer_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsViewViews);
};

#endif  
