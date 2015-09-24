// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_GTK_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_GTK_H_

#include <gdk/gdk.h>

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "content/browser/accessibility/browser_accessibility_manager.h"
#include "content/browser/renderer_host/gtk_plugin_container_manager.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/common/content_export.h"
#include "ipc/ipc_sender.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/base/x/active_window_watcher_x_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "webkit/common/cursors/webcursor.h"

typedef struct _GtkClipboard GtkClipboard;
typedef struct _GtkSelectionData GtkSelectionData;

namespace content {
class GtkIMContextWrapper;
class GtkKeyBindingsHandler;
class RenderWidgetHost;
class RenderWidgetHostImpl;
struct NativeWebKeyboardEvent;

class CONTENT_EXPORT RenderWidgetHostViewGtk
    : public RenderWidgetHostViewBase,
      public BrowserAccessibilityDelegate,
      public ui::ActiveWindowWatcherXObserver,
      public IPC::Sender {
 public:
  virtual ~RenderWidgetHostViewGtk();

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void InitAsChild(gfx::NativeView parent_view) OVERRIDE;
  virtual RenderWidgetHost* GetRenderWidgetHost() const OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual void SetBounds(const gfx::Rect& rect) OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeViewId GetNativeViewId() const OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;
  virtual bool HasFocus() const OVERRIDE;
  virtual bool IsSurfaceAvailableForCopy() const OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsShowing() OVERRIDE;
  virtual gfx::Rect GetViewBounds() const OVERRIDE;
  virtual GdkEventButton* GetLastMouseDown() OVERRIDE;
  virtual gfx::NativeView BuildInputMethodsGtkMenu() OVERRIDE;
  virtual void SetBackground(const SkBitmap& background) OVERRIDE;

  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos) OVERRIDE;
  virtual void InitAsFullscreen(
      RenderWidgetHostView* reference_host_view) OVERRIDE;
  virtual void WasShown() OVERRIDE;
  virtual void WasHidden() OVERRIDE;
  virtual void MovePluginWindows(
      const gfx::Vector2d& scroll_offset,
      const std::vector<WebPluginGeometry>& moves) OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void Blur() OVERRIDE;
  virtual void UpdateCursor(const WebCursor& cursor) OVERRIDE;
  virtual void SetIsLoading(bool is_loading) OVERRIDE;
  virtual void TextInputTypeChanged(ui::TextInputType type,
                                    ui::TextInputMode input_mode,
                                    bool can_compose_inline) OVERRIDE;
  virtual void ImeCancelComposition() OVERRIDE;
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect,
      const gfx::Vector2d& scroll_delta,
      const std::vector<gfx::Rect>& copy_rects,
      const ui::LatencyInfo& latency_info) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status,
                                 int error_code) OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh) {}
  virtual void SetTooltipText(const base::string16& tooltip_text) OVERRIDE;
  virtual void SelectionChanged(const base::string16& text,
                                size_t offset,
                                const gfx::Range& range) OVERRIDE;
  virtual void SelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params) OVERRIDE;
  virtual void ScrollOffsetChanged() OVERRIDE;
  virtual BackingStore* AllocBackingStore(const gfx::Size& size) OVERRIDE;
  virtual void CopyFromCompositingSurface(
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      const base::Callback<void(bool, const SkBitmap&)>& callback) OVERRIDE;
  virtual void CopyFromCompositingSurfaceToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual bool CanCopyToVideoFrame() const OVERRIDE;
  virtual void OnAcceleratedCompositingStateChange() OVERRIDE;
  virtual void AcceleratedSurfaceInitialized(int host_id,
                                             int route_id) OVERRIDE;
  virtual void AcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfaceSuspend() OVERRIDE;
  virtual void AcceleratedSurfaceRelease() OVERRIDE;
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) OVERRIDE;
  virtual void SetHasHorizontalScrollbar(
      bool has_horizontal_scrollbar) OVERRIDE;
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) OVERRIDE;
  virtual void GetScreenInfo(blink::WebScreenInfo* results) OVERRIDE;
  virtual gfx::Rect GetBoundsInRootWindow() OVERRIDE;
  virtual gfx::GLSurfaceHandle GetCompositingSurface() OVERRIDE;
  virtual void ResizeCompositingSurface(const gfx::Size&) OVERRIDE;
  virtual bool LockMouse() OVERRIDE;
  virtual void UnlockMouse() OVERRIDE;
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params)
      OVERRIDE;

  
  virtual void ActiveWindowChanged(GdkWindow* active_window) OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  
  
  void ModifyEventForEdgeDragging(GtkWidget* widget, GdkEventMotion* event);

  
  
  
  void ModifyEventMovementAndCoords(blink::WebMouseEvent* event);

  void Paint(const gfx::Rect&);

  
  
  
  
  
  
  void ForwardKeyboardEvent(const NativeWebKeyboardEvent& event);

  bool RetrieveSurrounding(std::string* text, size_t* cursor_index);

  
  virtual void SetAccessibilityFocus(int acc_obj_id) OVERRIDE;
  virtual void AccessibilityDoDefaultAction(int acc_obj_id) OVERRIDE;
  virtual void AccessibilityScrollToMakeVisible(
      int acc_obj_id, gfx::Rect subfocus) OVERRIDE;
  virtual void AccessibilityScrollToPoint(
      int acc_obj_id, gfx::Point point) OVERRIDE;
  virtual void AccessibilitySetTextSelection(
      int acc_obj_id, int start_offset, int end_offset) OVERRIDE;
  virtual gfx::Point GetLastTouchEventLocation() const OVERRIDE;
  virtual void FatalAccessibilityTreeError() OVERRIDE;

  
  AtkObject* GetAccessible();

 protected:
  friend class RenderWidgetHostView;

  
  explicit RenderWidgetHostViewGtk(RenderWidgetHost* widget);

 private:
  friend class RenderWidgetHostViewGtkWidget;

  CHROMEGTK_CALLBACK_0(RenderWidgetHostViewGtk,
                       void,
                       OnDestroy);

  
  
  bool NeedsInputGrab();

  
  
  bool IsPopup() const;

  
  void DoSharedInit();

  
  
  void DoPopupOrFullscreenInit(GtkWindow* window, const gfx::Rect& bounds);

  
  void ShowCurrentCursor();

  void set_last_mouse_down(GdkEventButton* event);

  
  void MarkCachedWidgetCenterStale();

  void OnCreatePluginContainer(gfx::PluginWindowHandle id);
  void OnDestroyPluginContainer(gfx::PluginWindowHandle id);

  gfx::Point GetWidgetCenter();

  
  RenderWidgetHostImpl* host_;

  
  ui::OwnedWidgetGtk view_;

  
  
  
  bool about_to_validate_and_paint_;

  
  gfx::Rect invalid_rect_;

  
  bool is_loading_;

  
  WebCursor current_cursor_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks web_contents_switch_paint_time_;

  
  GtkWidget* parent_;

  
  bool is_popup_first_mouse_release_;

  
  
  
  bool was_imcontext_focused_before_grab_;

  
  
  
  bool do_x_grab_;

  
  bool is_fullscreen_;

  
  
  bool made_active_;

  
  
  
  
  gfx::Point unlocked_mouse_position_;
  
  gfx::Point unlocked_global_mouse_position_;
  
  gfx::Point global_mouse_position_;
  
  bool mouse_has_been_warped_to_new_center_;
  
  
  bool mouse_is_being_warped_to_unlocked_position_;

  
  
  unsigned long destroy_handler_id_;

  
  scoped_ptr<GtkIMContextWrapper> im_context_;

  
  
  scoped_ptr<GtkKeyBindingsHandler> key_bindings_handler_;

  
  GtkPluginContainerManager plugin_container_manager_;

  
  
  gfx::Size requested_size_;

  
  gfx::Point widget_center_;
  
  bool widget_center_valid_;

  
  
  
  int dragged_at_horizontal_edge_;

  
  
  
  int dragged_at_vertical_edge_;

  gfx::PluginWindowHandle compositing_surface_;

  
  
  GdkEventButton* last_mouse_down_;

  
  
  scoped_ptr<BrowserAccessibilityManager> browser_accessibility_manager_;

  ui::GtkSignalRegistrar signals_;

  ui::LatencyInfo software_latency_info_;
};

}  

#endif  
