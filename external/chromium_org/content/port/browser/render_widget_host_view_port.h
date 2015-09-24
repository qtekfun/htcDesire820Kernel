// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PORT_BROWSER_RENDER_WIDGET_HOST_VIEW_PORT_H_
#define CONTENT_PORT_BROWSER_RENDER_WIDGET_HOST_VIEW_PORT_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/kill.h"
#include "base/strings/string16.h"
#include "cc/output/compositor_frame.h"
#include "content/common/content_export.h"
#include "content/port/browser/event_with_latency_info.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/public/browser/render_widget_host_view.h"
#include "ipc/ipc_listener.h"
#include "third_party/WebKit/public/web/WebPopupType.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/gfx/range/range.h"
#include "ui/surface/transport_dib.h"

class SkBitmap;
class WebCursor;

struct AccessibilityHostMsg_EventParams;
struct GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params;
struct GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params;
struct ViewHostMsg_TextInputState_Params;
struct ViewHostMsg_SelectionBounds_Params;

namespace media {
class VideoFrame;
}

namespace blink {
struct WebScreenInfo;
}

namespace content {
class BackingStore;
class RenderWidgetHostViewFrameSubscriber;
class SyntheticGesture;
class SyntheticGestureTarget;
struct WebPluginGeometry;
struct NativeWebKeyboardEvent;

class CONTENT_EXPORT RenderWidgetHostViewPort : public RenderWidgetHostView,
                                                public IPC::Listener {
 public:
  virtual ~RenderWidgetHostViewPort() {}

  
  static RenderWidgetHostViewPort* FromRWHV(RenderWidgetHostView* rwhv);

  
  static RenderWidgetHostViewPort* CreateViewForWidget(
      RenderWidgetHost* widget);

  static void GetDefaultScreenInfo(blink::WebScreenInfo* results);

  
  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos) = 0;

  
  
  
  
  virtual void InitAsFullscreen(
      RenderWidgetHostView* reference_host_view) = 0;

  
  virtual void WasShown() = 0;

  
  virtual void WasHidden() = 0;

  
  
  virtual void MovePluginWindows(
      const gfx::Vector2d& scroll_offset,
      const std::vector<WebPluginGeometry>& moves) = 0;

  
  virtual void Blur() = 0;

  
  virtual void UpdateCursor(const WebCursor& cursor) = 0;

  
  virtual void SetIsLoading(bool is_loading) = 0;

  
  virtual void TextInputTypeChanged(ui::TextInputType type,
                                    ui::TextInputMode mode,
                                    bool can_compose_inline) = 0;

  
  virtual void ImeCancelComposition() = 0;

#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  
  virtual void ImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds) = 0;
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect,
      const gfx::Vector2d& scroll_delta,
      const std::vector<gfx::Rect>& copy_rects,
      const ui::LatencyInfo& latency_info) = 0;

  
  virtual void RenderProcessGone(base::TerminationStatus status,
                                 int error_code) = 0;

  
  virtual void Destroy() = 0;

  
  
  virtual void SetTooltipText(const base::string16& tooltip_text) = 0;

  
  virtual void SelectionChanged(const base::string16& text,
                                size_t offset,
                                const gfx::Range& range) = 0;

  
  
  
  
  virtual void SelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params) = 0;

  
  virtual void ScrollOffsetChanged() = 0;

  
  virtual BackingStore* AllocBackingStore(const gfx::Size& size) = 0;

  
  
  
  // contents, scaled to |dst_size|, and written to |output|.
  
  
  
  
  
  virtual void CopyFromCompositingSurface(
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      const base::Callback<void(bool, const SkBitmap&)>& callback) = 0;

  
  
  
  
  
  
  
  virtual void CopyFromCompositingSurfaceToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback) = 0;

  
  
  
  
  
  
  virtual bool CanCopyToVideoFrame() const = 0;

  
  virtual bool CanSubscribeFrame() const = 0;

  
  
  
  virtual void BeginFrameSubscription(
      scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber) = 0;

  
  
  virtual void EndFrameSubscription() = 0;

  
  virtual void OnAcceleratedCompositingStateChange() = 0;
  
  virtual void AcceleratedSurfaceInitialized(int host_id, int route_id) = 0;
  
  
  
  
  
  virtual void AcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params_in_pixel,
      int gpu_host_id) = 0;
  
  
  virtual void AcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params_in_pixel,
      int gpu_host_id) = 0;

  
  
  virtual void AcceleratedSurfaceSuspend() = 0;

  virtual void AcceleratedSurfaceRelease() = 0;

  
  
  
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) = 0;

  virtual void OnSwapCompositorFrame(
      uint32 output_surface_id,
      scoped_ptr<cc::CompositorFrame> frame) = 0;

  virtual void GetScreenInfo(blink::WebScreenInfo* results) = 0;

  
  virtual gfx::Size GetPhysicalBackingSize() const = 0;

  
  
  virtual float GetOverdrawBottomHeight() const = 0;

  
  virtual gfx::Rect GetBoundsInRootWindow() = 0;

  virtual gfx::GLSurfaceHandle GetCompositingSurface() = 0;

  
  virtual void ResizeCompositingSurface(const gfx::Size&) = 0;

  
  
  
  
  
  virtual void ProcessAckedTouchEvent(const TouchEventWithLatencyInfo& touch,
                                      InputEventAckState ack_result) = 0;

  virtual void SetHasHorizontalScrollbar(bool has_horizontal_scrollbar) = 0;
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) = 0;

  
  virtual void UnhandledWheelEvent(const blink::WebMouseWheelEvent& event) = 0;

  
  
  
  
  virtual InputEventAckState FilterInputEvent(
      const blink::WebInputEvent& input_event) = 0;

  
  
  virtual void OnSetNeedsFlushInput() = 0;

  
  virtual void OnDidFlushInput() = 0;

  
  
  virtual scoped_ptr<SyntheticGestureTarget> CreateSyntheticGestureTarget() = 0;

  virtual void GestureEventAck(int gesture_event_type,
                               InputEventAckState ack_result) = 0;

  virtual void OnOverscrolled(gfx::Vector2dF accumulated_overscroll,
                              gfx::Vector2dF current_fling_velocity) = 0;

  virtual void SetPopupType(blink::WebPopupType popup_type) = 0;
  virtual blink::WebPopupType GetPopupType() = 0;

  virtual BrowserAccessibilityManager*
      GetBrowserAccessibilityManager() const = 0;
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params) = 0;

  
  
  virtual uint32 RendererFrameNumber() = 0;
  
  
  virtual void DidReceiveRendererFrame() = 0;

#if defined(OS_MACOSX)
  
  virtual void AboutToWaitForBackingStoreMsg() = 0;

  
  
  
  
  virtual bool PostProcessEventForPluginIme(
      const NativeWebKeyboardEvent& event) = 0;
#endif

#if defined(OS_ANDROID)
  virtual void ShowDisambiguationPopup(const gfx::Rect& target_rect,
                                       const SkBitmap& zoomed_bitmap) = 0;
  virtual void HasTouchEventHandlers(bool need_touch_events) = 0;
#endif

#if defined(OS_WIN) && !defined(USE_AURA)
  virtual void WillWmDestroy() = 0;
#endif

#if defined(OS_WIN) && defined(USE_AURA)
  virtual void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent) = 0;

  
  
  virtual gfx::NativeViewId GetParentForWindowlessPlugin() const = 0;
#endif
};

}  

#endif  
