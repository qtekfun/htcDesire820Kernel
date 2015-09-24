// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_GUEST_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_GUEST_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/common/content_export.h"
#include "ui/events/event.h"
#include "ui/events/gestures/gesture_recognizer.h"
#include "ui/events/gestures/gesture_types.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d_f.h"
#include "webkit/common/cursors/webcursor.h"

#if defined(TOOLKIT_GTK)
#include "content/browser/renderer_host/gtk_plugin_container_manager.h"
#endif  

namespace content {
class RenderWidgetHost;
class RenderWidgetHostImpl;
class BrowserPluginGuest;
struct NativeWebKeyboardEvent;

class CONTENT_EXPORT RenderWidgetHostViewGuest
    : public RenderWidgetHostViewBase,
      public ui::GestureConsumer,
      public ui::GestureEventHelper {
 public:
  RenderWidgetHostViewGuest(RenderWidgetHost* widget,
                            BrowserPluginGuest* guest,
                            RenderWidgetHostView* platform_view);
  virtual ~RenderWidgetHostViewGuest();

  
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
  virtual void SetBackground(const SkBitmap& background) OVERRIDE;
#if defined(OS_WIN) && !defined(USE_AURA)
  virtual void SetClickthroughRegion(SkRegion* region) OVERRIDE;
#endif

  
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
#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  virtual void ImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds) OVERRIDE;
#endif
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
  virtual void OnSwapCompositorFrame(
      uint32 output_surface_id,
      scoped_ptr<cc::CompositorFrame> frame) OVERRIDE;
  virtual void AcceleratedSurfaceSuspend() OVERRIDE;
  virtual void AcceleratedSurfaceRelease() OVERRIDE;
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) OVERRIDE;
  virtual void SetHasHorizontalScrollbar(
      bool has_horizontal_scrollbar) OVERRIDE;
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) OVERRIDE;
  virtual gfx::Rect GetBoundsInRootWindow() OVERRIDE;
  virtual gfx::GLSurfaceHandle GetCompositingSurface() OVERRIDE;
#if defined(OS_WIN) || defined(USE_AURA)
  virtual void ProcessAckedTouchEvent(
      const TouchEventWithLatencyInfo& touch,
      InputEventAckState ack_result) OVERRIDE;
#endif  
  virtual bool LockMouse() OVERRIDE;
  virtual void UnlockMouse() OVERRIDE;
  virtual void GetScreenInfo(blink::WebScreenInfo* results) OVERRIDE;
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>&
          params) OVERRIDE;

#if defined(OS_MACOSX)
  
  virtual void SetActive(bool active) OVERRIDE;
  virtual void SetTakesFocusOnlyOnMouseDown(bool flag) OVERRIDE;
  virtual void SetWindowVisibility(bool visible) OVERRIDE;
  virtual void WindowFrameChanged() OVERRIDE;
  virtual void ShowDefinitionForSelection() OVERRIDE;
  virtual bool SupportsSpeech() const OVERRIDE;
  virtual void SpeakSelection() OVERRIDE;
  virtual bool IsSpeaking() const OVERRIDE;
  virtual void StopSpeaking() OVERRIDE;

  
  virtual void AboutToWaitForBackingStoreMsg() OVERRIDE;
  virtual bool PostProcessEventForPluginIme(
      const NativeWebKeyboardEvent& event) OVERRIDE;
#endif  

#if defined(OS_ANDROID)
  
  virtual void ShowDisambiguationPopup(const gfx::Rect& target_rect,
                                       const SkBitmap& zoomed_bitmap) OVERRIDE;
  virtual void HasTouchEventHandlers(bool need_touch_events) OVERRIDE;
#endif  

#if defined(TOOLKIT_GTK)
  virtual GdkEventButton* GetLastMouseDown() OVERRIDE;
  virtual gfx::NativeView BuildInputMethodsGtkMenu() OVERRIDE;
#endif  

#if defined(OS_WIN) && !defined(USE_AURA)
  virtual void WillWmDestroy() OVERRIDE;
#endif  

#if defined(OS_WIN) && defined(USE_AURA)
  virtual void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent) OVERRIDE;
  virtual gfx::NativeViewId GetParentForWindowlessPlugin() const OVERRIDE;
#endif

  
  virtual bool CanDispatchToConsumer(ui::GestureConsumer* consumer) OVERRIDE;
  virtual void DispatchPostponedGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void DispatchCancelTouchEvent(ui::TouchEvent* event) OVERRIDE;

 protected:
  friend class RenderWidgetHostView;

 private:
  
  void DestroyGuestView();

  
  bool ForwardGestureEventToRenderer(ui::GestureEvent* gesture);

  
  void ProcessGestures(ui::GestureRecognizer::Gestures* gestures);

  
  RenderWidgetHostImpl* host_;

  BrowserPluginGuest *guest_;
  gfx::Size size_;
  
  
  
  RenderWidgetHostViewPort* platform_view_;
#if defined(OS_WIN) || defined(USE_AURA)
  scoped_ptr<ui::GestureRecognizer> gesture_recognizer_;
#endif  
  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewGuest);
};

}  

#endif  