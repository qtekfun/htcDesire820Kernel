// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_RENDER_WIDGET_H_
#define CONTENT_RENDERER_RENDER_WIDGET_H_

#include <deque>
#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "cc/debug/rendering_stats_instrumentation.h"
#include "content/common/browser_rendering_stats.h"
#include "content/common/content_export.h"
#include "content/common/gpu/client/webgraphicscontext3d_command_buffer_impl.h"
#include "content/common/input/synthetic_gesture_params.h"
#include "content/renderer/paint_aggregator.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "third_party/WebKit/public/platform/WebRect.h"
#include "third_party/WebKit/public/web/WebCompositionUnderline.h"
#include "third_party/WebKit/public/web/WebPopupType.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "third_party/WebKit/public/web/WebTextInputInfo.h"
#include "third_party/WebKit/public/web/WebTouchAction.h"
#include "third_party/WebKit/public/web/WebWidget.h"
#include "third_party/WebKit/public/web/WebWidgetClient.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"
#include "ui/gfx/vector2d_f.h"
#include "ui/surface/transport_dib.h"
#include "webkit/common/cursors/webcursor.h"

struct ViewHostMsg_UpdateRect_Params;
struct ViewMsg_Resize_Params;
class ViewHostMsg_UpdateRect;

namespace IPC {
class SyncMessage;
}

namespace blink {
class WebGestureEvent;
class WebInputEvent;
class WebKeyboardEvent;
class WebMouseEvent;
class WebTouchEvent;
struct WebRenderingStatsImpl;
}

namespace cc { class OutputSurface; }

namespace gfx {
class Range;
}

namespace content {
class ExternalPopupMenu;
class PepperPluginInstanceImpl;
class RenderWidgetCompositor;
class RenderWidgetTest;
class ResizingModeSelector;
struct ContextMenuParams;
struct GpuRenderingStats;
struct WebPluginGeometry;

class CONTENT_EXPORT RenderWidget
    : public IPC::Listener,
      public IPC::Sender,
      NON_EXPORTED_BASE(virtual public blink::WebWidgetClient),
      public base::RefCounted<RenderWidget> {
 public:
  
  
  static RenderWidget* Create(int32 opener_id,
                              blink::WebPopupType popup_type,
                              const blink::WebScreenInfo& screen_info);

  
  static blink::WebWidget* CreateWebWidget(RenderWidget* render_widget);

  
  
  
  
  
  
  int32 routing_id() const {
    return routing_id_;
  }

  int32 surface_id() const {
    return surface_id_;
  }

  
  blink::WebWidget* webwidget() const { return webwidget_; }

  gfx::Size size() const { return size_; }
  bool has_focus() const { return has_focus_; }
  bool is_fullscreen() const { return is_fullscreen_; }
  bool is_hidden() const { return is_hidden_; }

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual void suppressCompositorScheduling(bool enable);
  virtual void willBeginCompositorFrame();
  virtual void didInvalidateRect(const blink::WebRect&);
  virtual void didScrollRect(int dx, int dy,
                             const blink::WebRect& clipRect);
  virtual void didAutoResize(const blink::WebSize& new_size);
  virtual void didActivateCompositor(int input_handler_identifier);
  virtual void didDeactivateCompositor();
  virtual void initializeLayerTreeView();
  virtual blink::WebLayerTreeView* layerTreeView();
  virtual void didBecomeReadyForAdditionalInput();
  virtual void didCommitAndDrawCompositorFrame();
  virtual void didCompleteSwapBuffers();
  virtual void scheduleComposite();
  virtual void scheduleAnimation();
  virtual void didFocus();
  virtual void didBlur();
  virtual void didChangeCursor(const blink::WebCursorInfo&);
  virtual void closeWidgetSoon();
  virtual void show(blink::WebNavigationPolicy);
  virtual void runModal() {}
  virtual blink::WebRect windowRect();
  virtual void setToolTipText(const blink::WebString& text,
                              blink::WebTextDirection hint);
  virtual void setWindowRect(const blink::WebRect&);
  virtual blink::WebRect windowResizerRect();
  virtual blink::WebRect rootWindowRect();
  virtual blink::WebScreenInfo screenInfo();
  virtual float deviceScaleFactor();
  virtual void resetInputMethod();
  virtual void didHandleGestureEvent(const blink::WebGestureEvent& event,
                                     bool event_cancelled);

  
  
  void SchedulePluginMove(const WebPluginGeometry& move);

  
  
  void CleanupWindowInPluginMoves(gfx::PluginWindowHandle window);

  
  
  void GetRenderingStats(blink::WebRenderingStatsImpl&) const;

  
  
  
  
  bool GetGpuRenderingStats(GpuRenderingStats*) const;

  void GetBrowserRenderingStats(BrowserRenderingStats* stats);

  RenderWidgetCompositor* compositor() const;

  virtual scoped_ptr<cc::OutputSurface> CreateOutputSurface(bool fallback);

  
  typedef base::Callback<void()> SyntheticGestureCompletionCallback;

  
  
  void QueueSyntheticGesture(
      scoped_ptr<SyntheticGestureParams> gesture_params,
      const SyntheticGestureCompletionCallback& callback);

  
  virtual void Close();

  
  virtual void DidCommitCompositorFrame();

  float filtered_time_per_frame() const {
    return filtered_time_per_frame_;
  }

  
  void StartHandlingImeEvent();
  void FinishHandlingImeEvent();

  virtual void InstrumentWillBeginFrame(int frame_id) {}
  virtual void InstrumentDidBeginFrame() {}
  virtual void InstrumentDidCancelFrame() {}
  virtual void InstrumentWillComposite() {}

  virtual bool AllowPartialSwap() const;
  bool UsingSynchronousRendererCompositor() const;

  bool is_swapped_out() { return is_swapped_out_; }

  
  
  
  
  class ScreenMetricsEmulator;

  
  
  void EnableScreenMetricsEmulation(
      const gfx::Rect& device_rect,
      const gfx::Rect& widget_rect,
      float device_scale_factor,
      bool fit_to_view);
  void DisableScreenMetricsEmulation();
  void SetPopupOriginAdjustmentsForEmulation(ScreenMetricsEmulator* emulator);

  void ScheduleCompositeWithForcedRedraw();

  
  
  void OnSwapBuffersPosted();
  void OnSwapBuffersComplete();
  void OnSwapBuffersAborted();

  
  
  void UpdateTextInputType();

  
  
  void UpdateSelectionBounds();


#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  
  
  
  void UpdateCompositionInfo(bool should_update_range);
#endif

 protected:
  
  
  friend class base::RefCounted<RenderWidget>;
  
  friend class RenderWidgetTest;

  enum ResizeAck {
    SEND_RESIZE_ACK,
    NO_RESIZE_ACK,
  };

  RenderWidget(blink::WebPopupType popup_type,
               const blink::WebScreenInfo& screen_info,
               bool swapped_out,
               bool hidden);

  virtual ~RenderWidget();

  
  
  bool Init(int32 opener_id);

  
  bool DoInit(int32 opener_id,
              blink::WebWidget* web_widget,
              IPC::SyncMessage* create_widget_message);

  
  void CompleteInit();

  
  
  
  
  void SetSwappedOut(bool is_swapped_out);

  
  
  
  void PaintRect(const gfx::Rect& rect, const gfx::Point& canvas_origin,
                 SkCanvas* canvas);

  
  void PaintDebugBorder(const gfx::Rect& rect, SkCanvas* canvas);

  bool IsRenderingVSynced();
  void AnimationCallback();
  void AnimateIfNeeded();
  void InvalidationCallback();
  void FlushPendingInputEventAck();
  void DoDeferredUpdateAndSendInputAck();
  void DoDeferredUpdate();
  void DoDeferredClose();
  void DoDeferredSetWindowRect(const blink::WebRect& pos);
  virtual void Composite(base::TimeTicks frame_begin_time);

  
  
  
  virtual void SetBackground(const SkBitmap& bitmap);

  
  void Resize(const gfx::Size& new_size,
              const gfx::Size& physical_backing_size,
              float overdraw_bottom_height,
              const gfx::Rect& resizer_rect,
              bool is_fullscreen,
              ResizeAck resize_ack);
  
  void ResizeSynchronously(const gfx::Rect& new_position);
  virtual void SetScreenMetricsEmulationParameters(
      float device_scale_factor,
      const gfx::Point& root_layer_offset,
      float root_layer_scale);
  void SetExternalPopupOriginAdjustmentsForEmulation(
      ExternalPopupMenu* popup, ScreenMetricsEmulator* emulator);
  virtual void OnShowHostContextMenu(ContextMenuParams* params);

  
  void OnHandleInputEvent(const blink::WebInputEvent* event,
                          ui::LatencyInfo latency_info,
                          bool keyboard_shortcut);
  void OnCursorVisibilityChange(bool is_visible);
  void OnMouseCaptureLost();
  virtual void OnSetFocus(bool enable);
  void OnClose();
  void OnCreatingNewAck();
  virtual void OnResize(const ViewMsg_Resize_Params& params);
  void OnChangeResizeRect(const gfx::Rect& resizer_rect);
  virtual void OnWasHidden();
  virtual void OnWasShown(bool needs_repainting);
  virtual void OnWasSwappedOut();
  void OnUpdateRectAck();
  void OnCreateVideoAck(int32 video_id);
  void OnUpdateVideoAck(int32 video_id);
  void OnRequestMoveAck();
  void OnSetInputMethodActive(bool is_active);
  void OnCandidateWindowShown();
  void OnCandidateWindowUpdated();
  void OnCandidateWindowHidden();
  virtual void OnImeSetComposition(
      const base::string16& text,
      const std::vector<blink::WebCompositionUnderline>& underlines,
      int selection_start,
      int selection_end);
  virtual void OnImeConfirmComposition(const base::string16& text,
                                       const gfx::Range& replacement_range,
                                       bool keep_selection);
  void OnPaintAtSize(const TransportDIB::Handle& dib_id,
                     int tag,
                     const gfx::Size& page_size,
                     const gfx::Size& desired_size);
  void OnRepaint(gfx::Size size_to_paint);
  void OnSyntheticGestureCompleted();
  void OnSetTextDirection(blink::WebTextDirection direction);
  void OnGetFPS();
  void OnUpdateScreenRects(const gfx::Rect& view_screen_rect,
                           const gfx::Rect& window_screen_rect);
#if defined(OS_ANDROID)
  void OnShowImeIfNeeded();

  
  
  
  
  void IncrementOutstandingImeEventAcks();

  
  void OnImeEventAck();
#endif
  
  bool ShouldHandleImeEvent();

  void OnSnapshot(const gfx::Rect& src_subrect);
  void OnSetBrowserRenderingStats(const BrowserRenderingStats& stats);

  
  
  
  void AutoResizeCompositor();

  virtual void SetDeviceScaleFactor(float device_scale_factor);

  
  
  
  
  
  
  virtual void DidInitiatePaint() {}
  virtual void DidFlushPaint() {}

  
  
  
  
  
  virtual bool SupportsAsynchronousSwapBuffers();
  virtual GURL GetURLForGraphicsContext3D();

  virtual bool ForceCompositingModeEnabled();

  
  
  
  
  
  
  
  
  
  
  virtual PepperPluginInstanceImpl* GetBitmapForOptimizedPluginPaint(
      const gfx::Rect& paint_bounds,
      TransportDIB** dib,
      gfx::Rect* location,
      gfx::Rect* clip,
      float* scale_factor);

  
  
  virtual gfx::Vector2d GetScrollOffset();

  
  
  
  void SetHidden(bool hidden);

  void WillToggleFullscreen();
  void DidToggleFullscreen();

  bool next_paint_is_resize_ack() const;
  bool next_paint_is_restore_ack() const;
  void set_next_paint_is_resize_ack();
  void set_next_paint_is_restore_ack();
  void set_next_paint_is_repaint_ack();

#if defined(OS_ANDROID)
  
  
  
  
  
  
  void UpdateTextInputState(bool show_ime_if_needed, bool send_ime_ack);
#endif

  
  
  virtual ui::TextInputType GetTextInputType();
  virtual void GetSelectionBounds(gfx::Rect* start, gfx::Rect* end);
  virtual ui::TextInputType WebKitToUiTextInputType(
      blink::WebTextInputType type);

#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  
  
  
  
  virtual void GetCompositionCharacterBounds(
      std::vector<gfx::Rect>* character_bounds);

  
  
  virtual void GetCompositionRange(gfx::Range* range);

  
  
  bool ShouldUpdateCompositionInfo(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& bounds);
#endif

  
  
  virtual bool CanComposeInline();

  
  
  void ClearFocus();

  
  
  
  
  
  
  void SetPendingWindowRect(const blink::WebRect& r);

  
  
  virtual void DidHandleKeyEvent() {}

  
  
  
  
  virtual bool WillHandleMouseEvent(const blink::WebMouseEvent& event);

  
  
  
  
  virtual bool WillHandleKeyEvent(const blink::WebKeyboardEvent& event);

  
  
  
  
  virtual bool WillHandleGestureEvent(const blink::WebGestureEvent& event);

  
  
  virtual void DidHandleMouseEvent(const blink::WebMouseEvent& event) {}

  
  
  virtual void DidHandleTouchEvent(const blink::WebTouchEvent& event) {}

  
  
  virtual bool HasTouchEventHandlersAt(const gfx::Point& point) const;

  
  virtual void hasTouchEventHandlers(bool has_handlers);

  
  virtual void setTouchAction(blink::WebTouchAction touch_action);

  
  scoped_ptr<WebGraphicsContext3DCommandBufferImpl> CreateGraphicsContext3D(
      const blink::WebGraphicsContext3D::Attributes& attributes);

  bool OnSnapshotHelper(const gfx::Rect& src_subrect, SkBitmap* bitmap);

  
  
  int32 routing_id_;

  int32 surface_id_;

  
  blink::WebWidget* webwidget_;

  
  scoped_ptr<RenderWidgetCompositor> compositor_;

  
  
  
  
  
  
  
  int32 opener_id_;

  
  gfx::Rect initial_pos_;

  bool init_complete_;

  
  
  WebCursor current_cursor_;

  
  gfx::Size size_;

  
  TransportDIB* current_paint_buf_;

  PaintAggregator paint_aggregator_;

  
  gfx::Size physical_backing_size_;

  
  
  float overdraw_bottom_height_;

  
  gfx::Rect resizer_rect_;

  
  int next_paint_flags_;

  
  float filtered_time_per_frame_;

  
  
  bool update_reply_pending_;

  
  
  bool auto_resize_mode_;

  
  
  bool need_update_rect_for_auto_resize_;

  
  
  bool using_asynchronous_swapbuffers_;

  
  
  
  
  int num_swapbuffers_complete_pending_;

  
  
  
  static const int kMaxSwapBuffersPending = 2;

  
  bool did_show_;

  
  bool is_hidden_;

  
  bool is_fullscreen_;

  
  
  
  
  
  bool needs_repainting_on_restore_;

  
  bool has_focus_;

  
  bool handling_input_event_;

  
  bool handling_ime_event_;

  
  bool handling_touchstart_event_;

  
  
  bool closing_;

  
  
  
  bool is_swapped_out_;

  
  bool input_method_is_active_;

  
  blink::WebTextInputInfo text_input_info_;

  
  ui::TextInputType text_input_type_;

  
  ui::TextInputMode text_input_mode_;

  
  bool can_compose_inline_;

  
  gfx::Rect selection_focus_rect_;
  gfx::Rect selection_anchor_rect_;

  
  std::vector<gfx::Rect> composition_character_bounds_;

  
  gfx::Range composition_range_;

  
  blink::WebPopupType popup_type_;

  
  typedef std::vector<WebPluginGeometry> WebPluginGeometryVector;
  WebPluginGeometryVector plugin_window_moves_;

  
  SkBitmap background_;

  
  
  int pending_window_rect_count_;
  blink::WebRect pending_window_rect_;

  
  gfx::Rect view_screen_rect_;
  gfx::Rect window_screen_rect_;

  scoped_ptr<IPC::Message> pending_input_event_ack_;

  
  base::TimeDelta total_input_handling_time_this_frame_;

  
  bool suppress_next_char_events_;

  
  
  bool is_accelerated_compositing_active_;

  
  
  
  bool was_accelerated_compositing_ever_active_;

  base::OneShotTimer<RenderWidget> animation_timer_;
  base::Time animation_floor_time_;
  bool animation_update_pending_;
  bool invalidation_task_posted_;

  bool has_disable_gpu_vsync_switch_;
  base::TimeTicks last_do_deferred_update_time_;

  
  scoped_ptr<cc::RenderingStatsInstrumentation> legacy_software_mode_stats_;

  
  
  scoped_ptr<ViewHostMsg_UpdateRect_Params> pending_update_params_;

  
  
  
  
  
  std::deque<ViewHostMsg_UpdateRect*> updates_pending_swap_;

  
  blink::WebScreenInfo screen_info_;

  
  
  float device_scale_factor_;

  
  
  
  std::queue<SyntheticGestureCompletionCallback>
      pending_synthetic_gesture_callbacks_;

  
  bool is_threaded_compositing_enabled_;

  
  
  BrowserRenderingStats browser_rendering_stats_;

  
  
  ui::LatencyInfo latency_info_;

  uint32 next_output_surface_id_;

#if defined(OS_ANDROID)
  
  
  
  int outstanding_ime_acks_;
#endif

  scoped_ptr<ScreenMetricsEmulator> screen_metrics_emulator_;

  
  
  gfx::Point popup_view_origin_for_emulation_;
  gfx::Point popup_screen_origin_for_emulation_;
  float popup_origin_scale_for_emulation_;

  scoped_ptr<ResizingModeSelector> resizing_mode_selector_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidget);
};

}  

#endif  
