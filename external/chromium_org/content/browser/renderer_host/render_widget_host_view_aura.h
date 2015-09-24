// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_AURA_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_AURA_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "cc/layers/delegated_frame_provider.h"
#include "cc/layers/delegated_frame_resource_collection.h"
#include "cc/resources/texture_mailbox.h"
#include "content/browser/accessibility/browser_accessibility_manager.h"
#include "content/browser/aura/image_transport_factory.h"
#include "content/browser/aura/owned_mailbox.h"
#include "content/browser/renderer_host/delegated_frame_evictor.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/browser/renderer_host/software_frame_manager.h"
#include "content/common/content_export.h"
#include "content/common/gpu/client/gl_helper.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/client/activation_delegate.h"
#include "ui/aura/client/cursor_client_observer.h"
#include "ui/aura/client/focus_change_observer.h"
#include "ui/aura/root_window_observer.h"
#include "ui/aura/window_delegate.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/compositor_observer.h"
#include "ui/gfx/display_observer.h"
#include "ui/gfx/rect.h"
#include "webkit/common/cursors/webcursor.h"

namespace aura {
class WindowTracker;
}

namespace cc {
class CopyOutputResult;
class DelegatedFrameData;
}

namespace gfx {
class Canvas;
class Display;
}

namespace ui {
class CompositorLock;
class InputMethod;
class Texture;
}

namespace content {
class RenderWidgetHostImpl;
class RenderWidgetHostView;
class ResizeLock;

class CONTENT_EXPORT RenderWidgetHostViewAura
    : public RenderWidgetHostViewBase,
      public ui::CompositorObserver,
      public ui::TextInputClient,
      public gfx::DisplayObserver,
      public aura::RootWindowObserver,
      public aura::WindowDelegate,
      public aura::client::ActivationDelegate,
      public aura::client::ActivationChangeObserver,
      public aura::client::FocusChangeObserver,
      public aura::client::CursorClientObserver,
      public ImageTransportFactoryObserver,
      public BrowserAccessibilityDelegate,
      public SoftwareFrameManagerClient,
      public DelegatedFrameEvictorClient,
      public base::SupportsWeakPtr<RenderWidgetHostViewAura>,
      public cc::DelegatedFrameResourceCollectionClient {
 public:
  
  class PaintObserver {
   public:
    PaintObserver() {}
    virtual ~PaintObserver() {}

    
    virtual void OnPaintComplete() = 0;

    
    virtual void OnCompositingComplete() = 0;

    
    virtual void OnUpdateCompositorContent() = 0;

    
    virtual void OnPageLoadComplete() = 0;

    
    
    virtual void OnViewDestroyed() = 0;
  };

  
  class TouchEditingClient {
   public:
    TouchEditingClient() {}

    
    virtual void StartTouchEditing() = 0;

    
    virtual void EndTouchEditing() = 0;

    
    virtual void OnSelectionOrCursorChanged(const gfx::Rect& anchor,
                                            const gfx::Rect& focus) = 0;

    
    virtual void OnTextInputTypeChanged(ui::TextInputType type) = 0;

    
    
    virtual bool HandleInputEvent(const ui::Event* event) = 0;

    
    virtual void GestureEventAck(int gesture_event_type) = 0;

    
    
    virtual void OnViewDestroyed() = 0;

   protected:
    virtual ~TouchEditingClient() {}
  };

  void set_paint_observer(PaintObserver* observer) {
    paint_observer_ = observer;
  }

  void set_touch_editing_client(TouchEditingClient* client) {
    touch_editing_client_ = client;
  }

  
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
  virtual void ImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds) OVERRIDE;
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect,
      const gfx::Vector2d& scroll_delta,
      const std::vector<gfx::Rect>& copy_rects,
      const ui::LatencyInfo& latency_info) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status,
                                 int error_code) OVERRIDE;
  virtual void Destroy() OVERRIDE;
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
  virtual bool CanSubscribeFrame() const OVERRIDE;
  virtual void BeginFrameSubscription(
      scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber) OVERRIDE;
  virtual void EndFrameSubscription() OVERRIDE;
  virtual void OnAcceleratedCompositingStateChange() OVERRIDE;
  virtual void AcceleratedSurfaceInitialized(int host_id,
                                             int route_id) OVERRIDE;
  virtual void AcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params_in_pixel,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params_in_pixel,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfaceSuspend() OVERRIDE;
  virtual void AcceleratedSurfaceRelease() OVERRIDE;
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) OVERRIDE;
  virtual void GetScreenInfo(blink::WebScreenInfo* results) OVERRIDE;
  virtual gfx::Rect GetBoundsInRootWindow() OVERRIDE;
  virtual void GestureEventAck(int gesture_event_type,
                               InputEventAckState ack_result) OVERRIDE;
  virtual void ProcessAckedTouchEvent(
      const TouchEventWithLatencyInfo& touch,
      InputEventAckState ack_result) OVERRIDE;
  virtual scoped_ptr<SyntheticGestureTarget> CreateSyntheticGestureTarget()
      OVERRIDE;
  virtual void SetHasHorizontalScrollbar(
      bool has_horizontal_scrollbar) OVERRIDE;
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) OVERRIDE;
  virtual gfx::GLSurfaceHandle GetCompositingSurface() OVERRIDE;
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>&
          params) OVERRIDE;
  virtual bool LockMouse() OVERRIDE;
  virtual void UnlockMouse() OVERRIDE;
  virtual void OnSwapCompositorFrame(
      uint32 output_surface_id,
      scoped_ptr<cc::CompositorFrame> frame) OVERRIDE;
#if defined(OS_WIN)
  virtual void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent) OVERRIDE;
  virtual gfx::NativeViewId GetParentForWindowlessPlugin() const OVERRIDE;
#endif

  
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const base::string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual gfx::NativeWindow GetAttachedWindow() const OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual ui::TextInputMode GetTextInputMode() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual gfx::Rect GetCaretBounds() const OVERRIDE;
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) const OVERRIDE;
  virtual bool HasCompositionText() const OVERRIDE;
  virtual bool GetTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetCompositionTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetSelectionRange(gfx::Range* range) const OVERRIDE;
  virtual bool SetSelectionRange(const gfx::Range& range) OVERRIDE;
  virtual bool DeleteRange(const gfx::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(const gfx::Range& range,
                                base::string16* text) const OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) OVERRIDE;
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE;
  virtual void OnCandidateWindowShown() OVERRIDE;
  virtual void OnCandidateWindowUpdated() OVERRIDE;
  virtual void OnCandidateWindowHidden() OVERRIDE;

  
  virtual void OnDisplayBoundsChanged(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& new_display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;

  
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
  virtual void DidRecreateLayer(ui::Layer *old_layer,
                                ui::Layer *new_layer) OVERRIDE;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual bool ShouldActivate() const OVERRIDE;

  
  virtual void OnWindowActivated(aura::Window* gained_activation,
                                 aura::Window* lost_activation) OVERRIDE;

  
  virtual void OnCursorVisibilityChanged(bool is_visible) OVERRIDE;

  
  virtual void OnWindowFocused(aura::Window* gained_focus,
                               aura::Window* lost_focus) OVERRIDE;

  
  virtual void OnRootWindowHostMoved(const aura::RootWindow* root,
                                     const gfx::Point& new_origin) OVERRIDE;

  
  virtual void SoftwareFrameWasFreed(
      uint32 output_surface_id, unsigned frame_id) OVERRIDE;
  virtual void ReleaseReferencesToSoftwareFrame() OVERRIDE;

  bool CanCopyToBitmap() const;

#if defined(OS_WIN)
  
  
  
  void UpdateConstrainedWindowRects(const std::vector<gfx::Rect>& rects);
#endif

  
  
  
  bool IsClosing() const { return in_shutdown_; };

 protected:
  friend class RenderWidgetHostView;
  virtual ~RenderWidgetHostViewAura();

  
  explicit RenderWidgetHostViewAura(RenderWidgetHost* host);

  RenderWidgetHostViewFrameSubscriber* frame_subscriber() const {
    return frame_subscriber_.get();
  }

  virtual bool ShouldCreateResizeLock();
  virtual scoped_ptr<ResizeLock> CreateResizeLock(bool defer_compositor_lock);

  
  aura::Window* window() { return window_; }
  gfx::Size current_frame_size() const { return current_frame_size_; }

  
  virtual void OnCompositingDidCommit(ui::Compositor* compositor) OVERRIDE;
  virtual void OnCompositingStarted(ui::Compositor* compositor,
                                    base::TimeTicks start_time) OVERRIDE;
  virtual void OnCompositingEnded(ui::Compositor* compositor) OVERRIDE;
  virtual void OnCompositingAborted(ui::Compositor* compositor) OVERRIDE;
  virtual void OnCompositingLockStateChanged(
      ui::Compositor* compositor) OVERRIDE;
  virtual void OnUpdateVSyncParameters(ui::Compositor* compositor,
                                       base::TimeTicks timebase,
                                       base::TimeDelta interval) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest, SetCompositionText);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest, TouchEventState);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest, TouchEventSyncAsync);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest, SwapNotifiesWindow);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest,
                           SkippedDelegatedFrames);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest, OutputSurfaceIdChange);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest,
                           DiscardDelegatedFrames);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewAuraTest, SoftwareDPIChange);

  class WindowObserver;
  friend class WindowObserver;

  
  virtual void OnLostResources() OVERRIDE;

  
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

  void UpdateCursorIfOverSelf();
  bool ShouldSkipFrame(gfx::Size size_in_dip) const;

  
  
  
  void InternalSetBounds(const gfx::Rect& rect);

  
  
  void MaybeCreateResizeLock();

  
  void CheckResizeLock();

  void UpdateExternalTexture();
  ui::InputMethod* GetInputMethod() const;

  
  bool NeedsInputGrab();

  
  
  void FinishImeCompositionSession();

  
  
  void ModifyEventMovementAndCoords(blink::WebMouseEvent* event);

  
  
  void NotifyRendererOfCursorVisibilityState(bool is_visible);

  
  
  void SchedulePaintIfNotInClip(const gfx::Rect& rect, const gfx::Rect& clip);

  
  
  bool ShouldMoveToCenter();

  
  void RunOnCommitCallbacks();

  
  void AddOnCommitCallbackAndDisableLocks(const base::Closure& callback);

  
  void AddedToRootWindow();

  
  void RemovingFromRootWindow();

  
  
  void SetSurfaceNotInUseByCompositor(scoped_refptr<ui::Texture>);

  
  
  static void CopyFromCompositingSurfaceHasResult(
      const gfx::Size& dst_size_in_pixel,
      const base::Callback<void(bool, const SkBitmap&)>& callback,
      scoped_ptr<cc::CopyOutputResult> result);
  static void PrepareTextureCopyOutputResult(
      const gfx::Size& dst_size_in_pixel,
      const base::Callback<void(bool, const SkBitmap&)>& callback,
      scoped_ptr<cc::CopyOutputResult> result);
  static void PrepareBitmapCopyOutputResult(
      const gfx::Size& dst_size_in_pixel,
      const base::Callback<void(bool, const SkBitmap&)>& callback,
      scoped_ptr<cc::CopyOutputResult> result);
  static void CopyFromCompositingSurfaceHasResultForVideo(
      base::WeakPtr<RenderWidgetHostViewAura> rwhva,
      scoped_refptr<OwnedMailbox> subscriber_texture,
      scoped_refptr<media::VideoFrame> video_frame,
      const base::Callback<void(bool)>& callback,
      scoped_ptr<cc::CopyOutputResult> result);
  static void CopyFromCompositingSurfaceFinishedForVideo(
      base::WeakPtr<RenderWidgetHostViewAura> rwhva,
      const base::Callback<void(bool)>& callback,
      scoped_refptr<OwnedMailbox> subscriber_texture,
      scoped_ptr<cc::SingleReleaseCallback> release_callback,
      bool result);

  ui::Compositor* GetCompositor() const;

  
  void DetachFromInputMethod();

  
  void ApplyEventFilterForPopupExit(ui::MouseEvent* event);

  
  gfx::Rect ConvertRectToScreen(const gfx::Rect& rect) const;

  
  gfx::Rect ConvertRectFromScreen(const gfx::Rect& rect) const;

  typedef base::Callback<void(bool, const scoped_refptr<ui::Texture>&)>
      BufferPresentedCallback;

  
  
  void BuffersSwapped(const gfx::Size& surface_size,
                      const gfx::Rect& damage_rect,
                      float surface_scale_factor,
                      const std::string& mailbox_name,
                      const ui::LatencyInfo& latency_info,
                      const BufferPresentedCallback& ack_callback);

  bool SwapBuffersPrepare(const gfx::Rect& surface_rect,
                          float surface_scale_factor,
                          const gfx::Rect& damage_rect,
                          const std::string& mailbox_name,
                          const BufferPresentedCallback& ack_callback);

  void SwapBuffersCompleted(
      const BufferPresentedCallback& ack_callback,
      const scoped_refptr<ui::Texture>& texture_to_return);

  void SwapDelegatedFrame(
      uint32 output_surface_id,
      scoped_ptr<cc::DelegatedFrameData> frame_data,
      float frame_device_scale_factor,
      const ui::LatencyInfo& latency_info);
  void SendDelegatedFrameAck(uint32 output_surface_id);
  void SendReturnedDelegatedResources(uint32 output_surface_id);

  
  virtual void EvictDelegatedFrame() OVERRIDE;

  
  virtual void UnusedResourcesAreAvailable() OVERRIDE;

  void SwapSoftwareFrame(uint32 output_surface_id,
                         scoped_ptr<cc::SoftwareFrameData> frame_data,
                         float frame_device_scale_factor,
                         const ui::LatencyInfo& latency_info);
  void SendSoftwareFrameAck(uint32 output_surface_id);
  void SendReclaimSoftwareFrames();
  void ReleaseSoftwareFrame(uint32 output_surface_id,
                            unsigned software_frame_id);

  void DidReceiveFrameFromRenderer();

  BrowserAccessibilityManager* GetOrCreateBrowserAccessibilityManager();

  
  void SetKeyboardFocus();

  
  RenderWidgetHostImpl* host_;

  aura::Window* window_;

  scoped_ptr<WindowObserver> window_observer_;

  
  
  
  bool in_shutdown_;

  
  bool in_bounds_changed_;

  
  bool is_fullscreen_;

  
  RenderWidgetHostViewAura* popup_parent_host_view_;

  
  RenderWidgetHostViewAura* popup_child_host_view_;

  class EventFilterForPopupExit;
  friend class EventFilterForPopupExit;
  scoped_ptr<ui::EventHandler> event_filter_for_popup_exit_;

  
  bool is_loading_;

  
  WebCursor current_cursor_;

  
  
  
  blink::WebTouchEvent touch_event_;

  
  ui::TextInputType text_input_type_;
  
  ui::TextInputMode text_input_mode_;
  bool can_compose_inline_;

  
  gfx::Rect selection_anchor_rect_;
  gfx::Rect selection_focus_rect_;

  
  std::vector<gfx::Rect> composition_character_bounds_;

  
  bool has_composition_text_;

  
  bool accept_return_character_;

  
  base::string16 tooltip_;

  std::vector<base::Closure> on_compositing_did_commit_callbacks_;

  
  scoped_refptr<ui::Texture> current_surface_;

  
  scoped_ptr<SoftwareFrameManager> software_frame_manager_;

  
  
  
  uint32 last_output_surface_id_;

  
  
  int pending_delegated_ack_count_;

  
  SkRegion previous_damage_;

  
  SkRegion skipped_damage_;

  
  
  bool skipped_frames_;

  
  
  
  scoped_refptr<cc::DelegatedFrameResourceCollection> resource_collection_;

  
  scoped_refptr<cc::DelegatedFrameProvider> frame_provider_;

  
  
  
  gfx::Size last_swapped_surface_size_;
  float last_swapped_surface_scale_factor_;

  gfx::GLSurfaceHandle shared_surface_handle_;

  
  gfx::Canvas* paint_canvas_;

  
  
  
  
  gfx::Point unlocked_mouse_position_;
  
  gfx::Point unlocked_global_mouse_position_;
  
  gfx::Point global_mouse_position_;
  
  
  
  
  bool synthetic_move_sent_;

  
  
  
  bool accelerated_compositing_state_changed_;

  
  
  
  
  
  
  scoped_ptr<ResizeLock> resize_lock_;

  
  gfx::Size current_frame_size_;

  
  scoped_refptr<ui::CompositorLock> released_front_lock_;

  
  
  scoped_ptr<aura::WindowTracker> host_tracker_;

  enum CanLockCompositorState {
    YES,
    
    YES_DID_LOCK,
    
    NO_PENDING_RENDERER_FRAME,
    
    NO_PENDING_COMMIT,
  };
  CanLockCompositorState can_lock_compositor_;

  
  
  enum CursorVisibilityState {
    UNKNOWN,
    VISIBLE,
    NOT_VISIBLE,
  };
  CursorVisibilityState cursor_visibility_state_in_renderer_;

  
  
  
  PaintObserver* paint_observer_;

#if defined(OS_WIN)
  
  
  std::vector<gfx::Rect> constrained_rects_;

  typedef std::map<HWND, WebPluginGeometry> PluginWindowMoves;
  
  
  
  
  PluginWindowMoves plugin_window_moves_;
#endif

  base::TimeTicks last_draw_ended_;

  
  scoped_ptr<RenderWidgetHostViewFrameSubscriber> frame_subscriber_;
  std::vector<scoped_refptr<OwnedMailbox> > idle_frame_subscriber_textures_;

  
  scoped_ptr<content::ReadbackYUVInterface>
      yuv_readback_pipeline_;

  TouchEditingClient* touch_editing_client_;

  ui::LatencyInfo software_latency_info_;

  struct ReleasedFrameInfo {
    ReleasedFrameInfo(uint32 output_id, unsigned software_frame_id)
        : output_surface_id(output_id), frame_id(software_frame_id) {}
    uint32 output_surface_id;
    unsigned frame_id;
  };
  scoped_ptr<ReleasedFrameInfo> released_software_frame_;
  scoped_ptr<DelegatedFrameEvictor> delegated_frame_evictor_;

  base::WeakPtrFactory<RenderWidgetHostViewAura> weak_ptr_factory_;

#if defined(OS_WIN)
  
  
  
  HWND plugin_parent_window_;
#endif
  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewAura);
};

}  

#endif  
