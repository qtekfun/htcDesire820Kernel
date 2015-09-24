// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_IMPL_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_IMPL_H_

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/process/kill.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "build/build_config.h"
#include "content/browser/renderer_host/input/input_ack_handler.h"
#include "content/browser/renderer_host/input/input_router_client.h"
#include "content/common/browser_rendering_stats.h"
#include "content/common/input/synthetic_gesture_packet.h"
#include "content/common/view_message_enums.h"
#include "content/port/browser/event_with_latency_info.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/public/browser/render_widget_host.h"
#include "content/public/common/page_zoom.h"
#include "ipc/ipc_listener.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/events/latency_info.h"
#include "ui/gfx/native_widget_types.h"

class WebCursor;
struct AcceleratedSurfaceMsg_BufferPresented_Params;
struct ViewHostMsg_CompositorSurfaceBuffersSwapped_Params;
struct ViewHostMsg_UpdateRect_Params;
struct ViewHostMsg_TextInputState_Params;
struct ViewHostMsg_BeginSmoothScroll_Params;

namespace base {
class TimeTicks;
}

namespace cc {
class CompositorFrame;
class CompositorFrameAck;
}

namespace gfx {
class Range;
}

namespace ui {
class KeyEvent;
}

namespace blink {
class WebInputEvent;
class WebMouseEvent;
struct WebCompositionUnderline;
struct WebScreenInfo;
}

#if defined(OS_ANDROID)
namespace blink {
class WebLayer;
}
#endif

namespace content {
class BackingStore;
class InputRouter;
class MockRenderWidgetHost;
class OverscrollController;
class RenderWidgetHostDelegate;
class RenderWidgetHostViewPort;
class SyntheticGestureController;
class TimeoutMonitor;
struct EditCommand;

class CONTENT_EXPORT RenderWidgetHostImpl : virtual public RenderWidgetHost,
                                            public InputRouterClient,
                                            public InputAckHandler,
                                            public IPC::Listener {
 public:
  
  
  
  
  RenderWidgetHostImpl(RenderWidgetHostDelegate* delegate,
                       RenderProcessHost* process,
                       int routing_id,
                       bool hidden);
  virtual ~RenderWidgetHostImpl();

  
  static RenderWidgetHostImpl* FromID(int32 process_id, int32 routing_id);

  
  
  
  static scoped_ptr<RenderWidgetHostIterator> GetAllRenderWidgetHosts();

  
  
  
  static RenderWidgetHostImpl* From(RenderWidgetHost* rwh);

  void set_hung_renderer_delay_ms(const base::TimeDelta& timeout) {
    hung_renderer_delay_ms_ = timeout.InMilliseconds();
  }

  
  virtual void Undo() OVERRIDE;
  virtual void Redo() OVERRIDE;
  virtual void Cut() OVERRIDE;
  virtual void Copy() OVERRIDE;
  virtual void CopyToFindPboard() OVERRIDE;
  virtual void Paste() OVERRIDE;
  virtual void PasteAndMatchStyle() OVERRIDE;
  virtual void Delete() OVERRIDE;
  virtual void SelectAll() OVERRIDE;
  virtual void Unselect() OVERRIDE;
  virtual void UpdateTextDirection(blink::WebTextDirection direction) OVERRIDE;
  virtual void NotifyTextDirection() OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void Blur() OVERRIDE;
  virtual void SetActive(bool active) OVERRIDE;
  virtual void CopyFromBackingStore(
      const gfx::Rect& src_rect,
      const gfx::Size& accelerated_dst_size,
      const base::Callback<void(bool, const SkBitmap&)>& callback) OVERRIDE;
#if defined(TOOLKIT_GTK)
  virtual bool CopyFromBackingStoreToGtkWindow(const gfx::Rect& dest_rect,
                                               GdkWindow* target) OVERRIDE;
#elif defined(OS_MACOSX)
  virtual gfx::Size GetBackingStoreSize() OVERRIDE;
  virtual bool CopyFromBackingStoreToCGContext(const CGRect& dest_rect,
                                               CGContextRef target) OVERRIDE;
#endif
  virtual void EnableFullAccessibilityMode() OVERRIDE;
  virtual void ForwardMouseEvent(
      const blink::WebMouseEvent& mouse_event) OVERRIDE;
  virtual void ForwardWheelEvent(
      const blink::WebMouseWheelEvent& wheel_event) OVERRIDE;
  virtual void ForwardKeyboardEvent(
      const NativeWebKeyboardEvent& key_event) OVERRIDE;
  virtual const gfx::Vector2d& GetLastScrollOffset() const OVERRIDE;
  virtual RenderProcessHost* GetProcess() const OVERRIDE;
  virtual int GetRoutingID() const OVERRIDE;
  virtual RenderWidgetHostView* GetView() const OVERRIDE;
  virtual bool IsLoading() const OVERRIDE;
  virtual bool IsRenderView() const OVERRIDE;
  virtual void PaintAtSize(TransportDIB::Handle dib_handle,
                           int tag,
                           const gfx::Size& page_size,
                           const gfx::Size& desired_size) OVERRIDE;
  virtual void Replace(const base::string16& word) OVERRIDE;
  virtual void ReplaceMisspelling(const base::string16& word) OVERRIDE;
  virtual void ResizeRectChanged(const gfx::Rect& new_rect) OVERRIDE;
  virtual void RestartHangMonitorTimeout() OVERRIDE;
  virtual void SetIgnoreInputEvents(bool ignore_input_events) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void WasResized() OVERRIDE;
  virtual void AddKeyPressEventCallback(
      const KeyPressEventCallback& callback) OVERRIDE;
  virtual void RemoveKeyPressEventCallback(
      const KeyPressEventCallback& callback) OVERRIDE;
  virtual void AddMouseEventCallback(
      const MouseEventCallback& callback) OVERRIDE;
  virtual void RemoveMouseEventCallback(
      const MouseEventCallback& callback) OVERRIDE;
  virtual void GetWebScreenInfo(blink::WebScreenInfo* result) OVERRIDE;
  virtual void GetSnapshotFromRenderer(
      const gfx::Rect& src_subrect,
      const base::Callback<void(bool, const SkBitmap&)>& callback) OVERRIDE;

  const NativeWebKeyboardEvent* GetLastKeyboardEvent() const;

  
  void NotifyScreenInfoChanged();

  
  
  
  void InvalidateScreenInfo();

  
  void SetView(RenderWidgetHostView* view);

  int surface_id() const { return surface_id_; }

  bool empty() const { return current_size_.IsEmpty(); }

  
  
  
  virtual void Init();

  
  virtual void Shutdown();

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  
  void WasHidden();
  void WasShown();

  
  bool is_hidden() const { return is_hidden_; }

  
  
  virtual void GotFocus();
  virtual void LostCapture();

  
  virtual void LostMouseLock();

  
  void SendCursorVisibilityState(bool is_visible);

  
  bool is_accelerated_compositing_active() {
    return is_accelerated_compositing_active_;
  }

  
  void ViewDestroyed();

  
  void SetIsLoading(bool is_loading);

  
  
  
  bool TryGetBackingStore(const gfx::Size& desired_size,
                          BackingStore** backing_store);

  
  
  
  
  
  
  
  
  
  BackingStore* GetBackingStore(bool force_create);

  
  
  BackingStore* AllocBackingStore(const gfx::Size& size);

  
  
  
  void DonePaintingToBackingStore();

  
  
  
  bool ScheduleComposite();

  
  
  
  void StartHangMonitorTimeout(base::TimeDelta delay);

  
  
  void StopHangMonitorTimeout();

  
  
  void ForwardGestureEvent(const blink::WebGestureEvent& gesture_event);
  void ForwardGestureEventWithLatencyInfo(
      const blink::WebGestureEvent& gesture_event,
      const ui::LatencyInfo& ui_latency);
  void ForwardTouchEventWithLatencyInfo(
      const blink::WebTouchEvent& touch_event,
      const ui::LatencyInfo& ui_latency);
  void ForwardMouseEventWithLatencyInfo(
      const MouseEventWithLatencyInfo& mouse_event);
  void ForwardWheelEventWithLatencyInfo(
      const MouseWheelEventWithLatencyInfo& wheel_event);

  void CancelUpdateTextDirection();

  
  virtual void OnPointerEventActivate();

  
  
  
  
  
  
  void SetInputMethodActive(bool activate);

  
  void CandidateWindowShown();
  void CandidateWindowUpdated();
  void CandidateWindowHidden();

  
  
  
  
  
  

  
  
  
  
  
  
  
  
  void ImeSetComposition(
      const base::string16& text,
      const std::vector<blink::WebCompositionUnderline>& underlines,
      int selection_start,
      int selection_end);

  
  
  
  
  
  
  void ImeConfirmComposition(const base::string16& text,
                             const gfx::Range& replacement_range,
                             bool keep_selection);

  
  void ImeCancelComposition();

  
  
  void ExtendSelectionAndDelete(size_t before, size_t after);

  
  
  virtual gfx::Rect GetRootWindowResizerRect() const;

  bool ignore_input_events() const {
    return ignore_input_events_;
  }

  bool input_method_active() const {
    return input_method_active_;
  }

  
  
  bool IgnoreInputEvents() const;

  
  bool ShouldForwardTouchEvent() const;

  bool has_touch_handler() const { return has_touch_handler_; }

  
  
  void StartUserGesture();

  
  void SetBackground(const SkBitmap& background);

  
  
  void SetEditCommandsForNextKeyEvent(
      const std::vector<EditCommand>& commands);

  
  AccessibilityMode accessibility_mode() const {
    return accessibility_mode_;
  }

  
  void SetAccessibilityMode(AccessibilityMode mode);

  
  
  void AccessibilityDoDefaultAction(int object_id);

  
  void AccessibilitySetFocus(int object_id);

  
  
  
  
  
  void AccessibilityScrollToMakeVisible(
      int acc_obj_id, gfx::Rect subfocus);

  
  
  
  void AccessibilityScrollToPoint(int acc_obj_id, gfx::Point point);

  
  void AccessibilitySetTextSelection(
      int acc_obj_id, int start_offset, int end_offset);

  
  void FatalAccessibilityTreeError();

#if defined(OS_WIN) && defined(USE_AURA)
  void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent);
  gfx::NativeViewAccessible GetParentNativeViewAccessible() const;
#endif

  
  void ExecuteEditCommand(const std::string& command,
                          const std::string& value);

  
  
  
  void ScrollFocusedEditableNodeIntoRect(const gfx::Rect& rect);

  
  void SelectRange(const gfx::Point& start, const gfx::Point& end);

  
  void MoveCaret(const gfx::Point& point);

  
  
  
  bool GotResponseToLockMouseRequest(bool allowed);

  
  
  
  
  virtual void UpdateVSyncParameters(base::TimeTicks timebase,
                                     base::TimeDelta interval);

  
  
  static void AcknowledgeBufferPresent(
      int32 route_id,
      int gpu_host_id,
      const AcceleratedSurfaceMsg_BufferPresented_Params& params);

  
  static void SendSwapCompositorFrameAck(
      int32 route_id,
      uint32 output_surface_id,
      int renderer_host_id,
      const cc::CompositorFrameAck& ack);

  
  static void SendReclaimCompositorResources(int32 route_id,
                                             uint32 output_surface_id,
                                             int renderer_host_id,
                                             const cc::CompositorFrameAck& ack);

  
  
  
  
  void AcknowledgeSwapBuffersToRenderer();

  bool is_threaded_compositing_enabled() const {
    return is_threaded_compositing_enabled_;
  }

#if defined(USE_AURA)
  
  
  void ParentChanged(gfx::NativeViewId new_parent);
#endif

  
  
  void CompositingSurfaceUpdated();

  void set_allow_privileged_mouse_lock(bool allow) {
    allow_privileged_mouse_lock_ = allow;
  }

  
  
  
  
  
  void ResetSizeAndRepaintPendingFlags();

  void DetachDelegate();

  
  void SendScreenRects();

  OverscrollController* overscroll_controller() const {
    return overscroll_controller_.get();
  }

  
  void SetOverscrollControllerEnabled(bool enabled);

  
  
  void SuppressNextCharEvents();

  
  void FlushInput();

  
  virtual void SetNeedsFlush() OVERRIDE;

  
  
  bool should_auto_resize() { return should_auto_resize_; }

  void ComputeTouchLatency(const ui::LatencyInfo& latency_info);
  void FrameSwapped(const ui::LatencyInfo& latency_info);
  void DidReceiveRendererFrame();

  
  
  int64 GetLatencyComponentId();

  static void CompositorFrameDrawn(const ui::LatencyInfo& latency_info);

  
  static void DisableResizeAckCheckForTesting();

 protected:
  virtual RenderWidgetHostImpl* AsRenderWidgetHostImpl() OVERRIDE;

  
  
  
  ui::LatencyInfo CreateRWHLatencyInfoIfNotExist(
      const ui::LatencyInfo* original, blink::WebInputEvent::Type type);

  
  
  
  void RendererExited(base::TerminationStatus status, int exit_code);

  
  
  gfx::NativeViewId GetNativeViewId() const;

  
  
  gfx::GLSurfaceHandle GetCompositingSurface();

  
  
  

  
  virtual void UnhandledWheelEvent(const blink::WebMouseWheelEvent& event) {}

  
  
  
  virtual void OnUserGesture() {}

  
  
  virtual void NotifyRendererUnresponsive() {}
  virtual void NotifyRendererResponsive() {}

  
  virtual void OnRenderAutoResized(const gfx::Size& new_size) {}

  

  
  
  
  
  virtual void RequestToLockMouse(bool user_gesture,
                                  bool last_unlocked_by_target);

  void RejectMouseLockOrUnlockIfNecessary();
  bool IsMouseLocked() const;

  
  virtual bool IsFullscreen() const;

  
  
  void SetShouldAutoResize(bool enable);

  
  
  int increment_in_flight_event_count() { return ++in_flight_event_count_; }
  int decrement_in_flight_event_count() { return --in_flight_event_count_; }

  
  bool IsInOverscrollGesture() const;

  
  
  
  
  
  RenderWidgetHostViewPort* view_;

  
  
  
  bool renderer_initialized_;

  
  int hung_renderer_delay_ms_;

 private:
  friend class MockRenderWidgetHost;

  
  void Destroy();

  
  void RendererIsUnresponsive();

  
  
  
  void RendererIsResponsive();

  
  void OnRenderViewReady();
  void OnRenderProcessGone(int status, int error_code);
  void OnClose();
  void OnUpdateScreenRectsAck();
  void OnRequestMove(const gfx::Rect& pos);
  void OnSetTooltipText(const base::string16& tooltip_text,
                        blink::WebTextDirection text_direction_hint);
  void OnPaintAtSizeAck(int tag, const gfx::Size& size);
#if defined(OS_MACOSX)
  void OnCompositorSurfaceBuffersSwapped(
      const ViewHostMsg_CompositorSurfaceBuffersSwapped_Params& params);
#endif
  bool OnSwapCompositorFrame(const IPC::Message& message);
  void OnOverscrolled(gfx::Vector2dF accumulated_overscroll,
                      gfx::Vector2dF current_fling_velocity);
  void OnUpdateRect(const ViewHostMsg_UpdateRect_Params& params);
  void OnUpdateIsDelayed();
  void OnQueueSyntheticGesture(const SyntheticGesturePacket& gesture_packet);
  virtual void OnFocus();
  virtual void OnBlur();
  void OnSetCursor(const WebCursor& cursor);
  void OnTextInputTypeChanged(ui::TextInputType type,
                              ui::TextInputMode input_mode,
                              bool can_compose_inline);
#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  void OnImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds);
#endif
  void OnImeCancelComposition();
  void OnDidActivateAcceleratedCompositing(bool activated);
  void OnLockMouse(bool user_gesture,
                   bool last_unlocked_by_target,
                   bool privileged);
  void OnUnlockMouse();
  void OnShowDisambiguationPopup(const gfx::Rect& rect,
                                 const gfx::Size& size,
                                 const TransportDIB::Id& id);
#if defined(OS_WIN)
  void OnWindowlessPluginDummyWindowCreated(
      gfx::NativeViewId dummy_activation_window);
  void OnWindowlessPluginDummyWindowDestroyed(
      gfx::NativeViewId dummy_activation_window);
#endif
  void OnSnapshot(bool success, const SkBitmap& bitmap);

  
  
  
  void DidUpdateBackingStore(const ViewHostMsg_UpdateRect_Params& params,
                             const base::TimeTicks& paint_start);

  
  
  
  
  bool PaintBackingStoreRect(TransportDIB::Id bitmap,
                             const gfx::Rect& bitmap_rect,
                             const std::vector<gfx::Rect>& copy_rects,
                             const gfx::Size& view_size,
                             float scale_factor,
                             const base::Closure& completion_callback);

  
  
  
  void ScrollBackingStoreRect(const gfx::Vector2d& delta,
                              const gfx::Rect& clip_rect,
                              const gfx::Size& view_size);

  
  
  bool KeyPressListenersHandleEvent(const NativeWebKeyboardEvent& event);

  
  virtual InputEventAckState FilterInputEvent(
      const blink::WebInputEvent& event,
      const ui::LatencyInfo& latency_info) OVERRIDE;
  virtual void IncrementInFlightEventCount() OVERRIDE;
  virtual void DecrementInFlightEventCount() OVERRIDE;
  virtual void OnHasTouchEventHandlers(bool has_handlers) OVERRIDE;
  virtual OverscrollController* GetOverscrollController() const OVERRIDE;
  virtual void DidFlush() OVERRIDE;

  
  virtual void OnKeyboardEventAck(const NativeWebKeyboardEvent& event,
                                  InputEventAckState ack_result) OVERRIDE;
  virtual void OnWheelEventAck(const MouseWheelEventWithLatencyInfo& event,
                               InputEventAckState ack_result) OVERRIDE;
  virtual void OnTouchEventAck(const TouchEventWithLatencyInfo& event,
                               InputEventAckState ack_result) OVERRIDE;
  virtual void OnGestureEventAck(const GestureEventWithLatencyInfo& event,
                                 InputEventAckState ack_result) OVERRIDE;
  virtual void OnUnexpectedEventAck(UnexpectedEventAckType type) OVERRIDE;

  
  
  void DelayedAutoResized();

  void WindowSnapshotReachedScreen(int snapshot_id);

  
  
  RenderWidgetHostDelegate* delegate_;

  
  
  
  RenderProcessHost* process_;

  
  int routing_id_;

  
  int surface_id_;

  
  bool is_loading_;

  
  bool is_hidden_;

  
  bool is_fullscreen_;

  
  bool is_accelerated_compositing_active_;

  
  bool is_threaded_compositing_enabled_;

  
  bool repaint_ack_pending_;

  
  bool resize_ack_pending_;

  
  
  scoped_ptr<blink::WebScreenInfo> screen_info_;

  
  
  bool screen_info_out_of_date_;

  
  gfx::Size current_size_;

  
  gfx::Size physical_backing_size_;

  
  
  float overdraw_bottom_height_;

  
  
  
  
  
  gfx::Size last_requested_size_;

  
  gfx::Size new_auto_size_;

  
  
  bool should_auto_resize_;

  bool waiting_for_screen_rects_ack_;
  gfx::Rect last_view_screen_rect_;
  gfx::Rect last_window_screen_rect_;

  AccessibilityMode accessibility_mode_;

  
  std::vector<KeyPressEventCallback> key_press_event_callbacks_;

  
  std::vector<MouseEventCallback> mouse_event_callbacks_;

  
  
  
  
  bool needs_repainting_on_restore_;

  
  bool is_unresponsive_;

  
  
  base::Time time_when_considered_hung_;

  
  
  int in_flight_event_count_;

  
  base::OneShotTimer<RenderWidgetHostImpl> hung_renderer_timer_;

  
  bool in_get_backing_store_;

  
  bool abort_get_backing_store_;

  
  bool view_being_painted_;

  
  
  base::TimeTicks repaint_start_time_;

  
  bool ignore_input_events_;

  
  bool input_method_active_;

  
  bool text_direction_updated_;
  blink::WebTextDirection text_direction_;

  
  
  
  bool text_direction_canceled_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool suppress_next_char_events_;

  
  gfx::Vector2d last_scroll_offset_;

  bool pending_mouse_lock_request_;
  bool allow_privileged_mouse_lock_;

  
  
  
  bool has_touch_handler_;

  base::WeakPtrFactory<RenderWidgetHostImpl> weak_factory_;

  scoped_ptr<SyntheticGestureController> synthetic_gesture_controller_;

  
  scoped_ptr<InputRouter> input_router_;

  scoped_ptr<OverscrollController> overscroll_controller_;

  scoped_ptr<TimeoutMonitor> hang_monitor_timeout_;

#if defined(OS_WIN)
  std::list<HWND> dummy_windows_for_activation_;
#endif

  
  std::queue<base::Callback<void(bool, const SkBitmap&)> > pending_snapshots_;

  int64 last_input_number_;

  BrowserRenderingStats rendering_stats_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostImpl);
};

}  

#endif  
