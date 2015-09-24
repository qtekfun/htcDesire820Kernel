// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_AURA_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_AURA_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/renderer_host/overscroll_controller_delegate.h"
#include "content/common/content_export.h"
#include "content/port/browser/render_view_host_delegate_view.h"
#include "content/port/browser/web_contents_view_port.h"
#include "ui/aura/client/drag_drop_delegate.h"
#include "ui/aura/window_delegate.h"
#include "ui/compositor/layer_animation_observer.h"

namespace aura {
class Window;
}

namespace ui {
class DropTargetEvent;
}

namespace content {
class OverscrollNavigationOverlay;
class ShadowLayerDelegate;
class TouchEditableImplAura;
class WebContentsViewDelegate;
class WebContentsImpl;
class WebDragDestDelegate;

class CONTENT_EXPORT WebContentsViewAura
    : public WebContentsViewPort,
      public RenderViewHostDelegateView,
      NON_EXPORTED_BASE(public OverscrollControllerDelegate),
      public ui::ImplicitAnimationObserver,
      public aura::WindowDelegate,
      public aura::client::DragDropDelegate {
 public:
  WebContentsViewAura(WebContentsImpl* web_contents,
                      WebContentsViewDelegate* delegate);

  void SetupOverlayWindowForTesting();

  void SetTouchEditableForTest(TouchEditableImplAura* touch_editable);

 private:
  class WindowObserver;

  virtual ~WebContentsViewAura();

  void SizeChangedCommon(const gfx::Size& size);

  void EndDrag(blink::WebDragOperationsMask ops);

  
  
  void PrepareOverscrollWindow();

  
  
  void PrepareContentWindowForOverscroll();

  
  
  
  void ResetOverscrollTransform();

  
  
  
  void CompleteOverscrollNavigation(OverscrollMode mode);

  
  
  
  aura::Window* GetWindowToAnimateForOverscroll();

  
  
  gfx::Vector2d GetTranslationForOverscroll(int delta_x, int delta_y);

  
  
  void PrepareOverscrollNavigationOverlay();

  
  
  void UpdateOverscrollWindowBrightness(float delta_x);

  void AttachTouchEditableToRenderView();

  void OverscrollUpdateForWebContentsDelegate(int delta_y);

  
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeView GetContentNativeView() const OVERRIDE;
  virtual gfx::NativeWindow GetTopLevelNativeWindow() const OVERRIDE;
  virtual void GetContainerBounds(gfx::Rect *out) const OVERRIDE;
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
                             blink::WebDragOperationsMask operations,
                             const gfx::ImageSkia& image,
                             const gfx::Vector2d& image_offset,
                             const DragEventSourceInfo& event_info) OVERRIDE;
  virtual void UpdateDragCursor(blink::WebDragOperation operation) OVERRIDE;
  virtual void GotFocus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;

  
  virtual gfx::Rect GetVisibleBounds() const OVERRIDE;
  virtual void OnOverscrollUpdate(float delta_x, float delta_y) OVERRIDE;
  virtual void OnOverscrollComplete(OverscrollMode overscroll_mode) OVERRIDE;
  virtual void OnOverscrollModeChange(OverscrollMode old_mode,
                                      OverscrollMode new_mode) OVERRIDE;

  
  virtual void OnImplicitAnimationsCompleted() OVERRIDE;

  
  virtual gfx::Size GetMinimumSize() const OVERRIDE;
  virtual gfx::Size GetMaximumSize() const OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& old_bounds,
                               const gfx::Rect& new_bounds) OVERRIDE;
  virtual gfx::NativeCursor GetCursor(const gfx::Point& point) OVERRIDE;
  virtual int GetNonClientComponent(const gfx::Point& point) const OVERRIDE;
  virtual bool ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) OVERRIDE;
  virtual bool CanFocus() OVERRIDE;
  virtual void OnCaptureLost() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void OnWindowDestroying() OVERRIDE;
  virtual void OnWindowDestroyed() OVERRIDE;
  virtual void OnWindowTargetVisibilityChanged(bool visible) OVERRIDE;
  virtual bool HasHitTestMask() const OVERRIDE;
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE;
  virtual void DidRecreateLayer(ui::Layer* old_layer,
                                ui::Layer* new_layer) OVERRIDE;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;

  
  virtual void OnDragEntered(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;

  scoped_ptr<aura::Window> window_;

  
  
  scoped_ptr<aura::Window> overscroll_window_;

  scoped_ptr<WindowObserver> window_observer_;

  
  WebContentsImpl* web_contents_;

  scoped_ptr<WebContentsViewDelegate> delegate_;

  blink::WebDragOperationsMask current_drag_op_;

  scoped_ptr<DropData> current_drop_data_;

  WebDragDestDelegate* drag_dest_delegate_;

  
  
  
  
  void* current_rvh_for_drag_;

  bool overscroll_change_brightness_;

  
  OverscrollMode current_overscroll_gesture_;

  
  
  OverscrollMode completed_overscroll_gesture_;

  
  
  scoped_ptr<OverscrollNavigationOverlay> navigation_overlay_;

  scoped_ptr<ShadowLayerDelegate> overscroll_shadow_;

  scoped_ptr<TouchEditableImplAura> touch_editable_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsViewAura);
};

}  

#endif  
