// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_H_

#import <Cocoa/Cocoa.h>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/browser/accessibility/browser_accessibility_delegate_mac.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/browser/renderer_host/software_frame_manager.h"
#include "content/common/edit_command.h"
#import "content/public/browser/render_widget_host_view_mac_base.h"
#include "ipc/ipc_sender.h"
#include "third_party/WebKit/public/web/WebCompositionUnderline.h"
#include "ui/base/cocoa/base_view.h"
#include "webkit/common/cursors/webcursor.h"

namespace content {
class CompositingIOSurfaceMac;
class CompositingIOSurfaceContext;
class RenderWidgetHostViewMac;
class RenderWidgetHostViewMacEditCommandHelper;
}

@class CompositingIOSurfaceLayer;
@class FullscreenWindowManager;
@protocol RenderWidgetHostViewMacDelegate;
@class ToolTip;

@protocol RenderWidgetHostViewMacOwner
- (content::RenderWidgetHostViewMac*)renderWidgetHostViewMac;
@end

@interface RenderWidgetHostViewCocoa
    : BaseView <RenderWidgetHostViewMacBase,
                RenderWidgetHostViewMacOwner,
                NSTextInputClient,
                BrowserAccessibilityDelegateCocoa> {
 @private
  scoped_ptr<content::RenderWidgetHostViewMac> renderWidgetHostView_;
  NSObject<RenderWidgetHostViewMacDelegate>* delegate_;  
  BOOL canBeKeyView_;
  BOOL takesFocusOnlyOnMouseDown_;
  BOOL closeOnDeactivate_;
  scoped_ptr<content::RenderWidgetHostViewMacEditCommandHelper>
      editCommand_helper_;

  
  id trackingRectOwner_;              
  void* trackingRectUserData_;
  NSTrackingRectTag lastToolTipTag_;
  base::scoped_nsobject<NSString> toolTip_;

  
  BOOL hasOpenMouseDown_;

  NSWindow* lastWindow_;  

  
  base::scoped_nsobject<NSCursor> currentCursor_;

  
  
  
  
  
  
  
  
  
  

  
  base::scoped_nsobject<NSArray> validAttributesForMarkedText_;

  
  BOOL handlingKeyDown_;

  
  BOOL hasMarkedText_;

  
  
  BOOL unmarkTextCalled_;

  
  
  
  
  NSRange markedRange_;

  
  NSRange selectedRange_;

  
  base::string16 textToBeInserted_;

  
  base::string16 markedText_;

  
  std::vector<blink::WebCompositionUnderline> underlines_;

  
  
  BOOL hasEditCommands_;

  
  
  
  content::EditCommands editCommands_;

  
  int focusedPluginIdentifier_;

  
  BOOL pluginImeActive_;

  
  BOOL mouseEventWasIgnored_;

  
  id endWheelMonitor_;

  

  
  BOOL handlingGlobalFrameDidChange_;

  
  float deviceScaleFactor_;

  
  
  
  
  BOOL suppressNextEscapeKeyUp_;
}

@property(nonatomic, readonly) NSRange selectedRange;
@property(nonatomic, readonly) BOOL suppressNextEscapeKeyUp;

- (void)setCanBeKeyView:(BOOL)can;
- (void)setTakesFocusOnlyOnMouseDown:(BOOL)b;
- (void)setCloseOnDeactivate:(BOOL)b;
- (void)setToolTipAtMousePoint:(NSString *)string;
- (BOOL)acceptsMouseEventsWhenInactive;
- (void)cancelComposition;
- (void)confirmComposition;
- (void)setPluginImeActive:(BOOL)active;
- (void)pluginFocusChanged:(BOOL)focused forPlugin:(int)pluginId;
- (BOOL)postProcessEventForPluginIme:(NSEvent*)event;
- (void)updateCursor:(NSCursor*)cursor;
- (NSRect)firstViewRectForCharacterRange:(NSRange)theRange
                             actualRange:(NSRangePointer)actualRange;
@end

namespace content {
class RenderWidgetHostImpl;

class RenderWidgetHostViewMac : public RenderWidgetHostViewBase,
                                public IPC::Sender,
                                public SoftwareFrameManagerClient {
 public:
  virtual ~RenderWidgetHostViewMac();

  RenderWidgetHostViewCocoa* cocoa_view() const { return cocoa_view_; }

  CONTENT_EXPORT void SetDelegate(
    NSObject<RenderWidgetHostViewMacDelegate>* delegate);
  void SetAllowOverlappingViews(bool overlapping);

  
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
  virtual void SetShowingContextMenu(bool showing) OVERRIDE;
  virtual void SetActive(bool active) OVERRIDE;
  virtual void SetTakesFocusOnlyOnMouseDown(bool flag) OVERRIDE;
  virtual void SetWindowVisibility(bool visible) OVERRIDE;
  virtual void WindowFrameChanged() OVERRIDE;
  virtual void ShowDefinitionForSelection() OVERRIDE;
  virtual bool SupportsSpeech() const OVERRIDE;
  virtual void SpeakSelection() OVERRIDE;
  virtual bool IsSpeaking() const OVERRIDE;
  virtual void StopSpeaking() OVERRIDE;
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
  virtual void OnSwapCompositorFrame(
      uint32 output_surface_id, scoped_ptr<cc::CompositorFrame> frame) OVERRIDE;
  virtual void OnAcceleratedCompositingStateChange() OVERRIDE;
  virtual void AcceleratedSurfaceInitialized(int host_id,
                                             int route_id) OVERRIDE;
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params
      ) OVERRIDE;
  virtual bool PostProcessEventForPluginIme(
      const NativeWebKeyboardEvent& event) OVERRIDE;

  virtual void AcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfaceSuspend() OVERRIDE;
  virtual void AcceleratedSurfaceRelease() OVERRIDE;
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) OVERRIDE;
  virtual void AboutToWaitForBackingStoreMsg() OVERRIDE;
  virtual void GetScreenInfo(blink::WebScreenInfo* results) OVERRIDE;
  virtual gfx::Rect GetBoundsInRootWindow() OVERRIDE;
  virtual gfx::GLSurfaceHandle GetCompositingSurface() OVERRIDE;

  virtual void SetHasHorizontalScrollbar(
      bool has_horizontal_scrollbar) OVERRIDE;
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) OVERRIDE;
  virtual bool LockMouse() OVERRIDE;
  virtual void UnlockMouse() OVERRIDE;
  virtual void UnhandledWheelEvent(
      const blink::WebMouseWheelEvent& event) OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  virtual void SoftwareFrameWasFreed(
      uint32 output_surface_id, unsigned frame_id) OVERRIDE;
  virtual void ReleaseReferencesToSoftwareFrame() OVERRIDE;

  
  void ForwardMouseEvent(const blink::WebMouseEvent& event);

  void KillSelf();

  void SetTextInputActive(bool active);

  
  void EnableCoreAnimation();

  
  void PluginImeCompositionCompleted(const base::string16& text, int plugin_id);

  const std::string& selected_text() const { return selected_text_; }

  
  
  void CompositorSwapBuffers(uint64 surface_handle,
                             const gfx::Size& size,
                             float scale_factor,
                             const ui::LatencyInfo& latency_info);

  
  bool DrawIOSurfaceWithoutCoreAnimation();

  
  void GotAcceleratedCompositingError();

  
  
  
  void SetOverlayView(RenderWidgetHostViewMac* overlay,
                      const gfx::Point& offset);

  
  void RemoveOverlayView();

  
  
  
  CONTENT_EXPORT bool GetCachedFirstRectForCharacterRange(
      NSRange range, NSRect* rect, NSRange* actual_range);

  
  
  
  bool GetLineBreakIndex(const std::vector<gfx::Rect>& bounds,
                         const gfx::Range& range,
                         size_t* line_break_point);

  
  
  
  gfx::Rect GetFirstRectForCompositionRange(const gfx::Range& range,
                                            gfx::Range* actual_range);

  
  
  gfx::Range ConvertCharacterRangeToCompositionRange(
      const gfx::Range& request_range);

  
  

  
  
  RenderWidgetHostImpl* render_widget_host_;

  
  
  bool about_to_validate_and_paint_;

  
  
  
  bool call_set_needs_display_in_rect_pending_;

  
  bool last_frame_was_accelerated_;

  
  
  
  NSRect invalid_rect_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks web_contents_switch_paint_time_;

  
  ui::TextInputType text_input_type_;
  bool can_compose_inline_;

  base::scoped_nsobject<CALayer> software_layer_;

  
  
  base::scoped_nsobject<CompositingIOSurfaceLayer> compositing_iosurface_layer_;
  scoped_ptr<CompositingIOSurfaceMac> compositing_iosurface_;
  scoped_refptr<CompositingIOSurfaceContext> compositing_iosurface_context_;

  
  scoped_ptr<SoftwareFrameManager> software_frame_manager_;

  
  
  bool software_frame_needs_to_send_ack_;

  
  bool allow_overlapping_views_;

  
  bool use_core_animation_;

  ui::LatencyInfo software_latency_info_;

  NSWindow* pepper_fullscreen_window() const {
    return pepper_fullscreen_window_;
  }

  CONTENT_EXPORT void release_pepper_fullscreen_window_for_testing();

  RenderWidgetHostViewMac* fullscreen_parent_host_view() const {
    return fullscreen_parent_host_view_;
  }

  RenderWidgetHostViewFrameSubscriber* frame_subscriber() const {
    return frame_subscriber_.get();
  }

  int window_number() const;

  float scale_factor() const;

  void FrameSwapped();

 private:
  friend class RenderWidgetHostView;
  friend class RenderWidgetHostViewMacTest;

  void GetVSyncParameters(
      base::TimeTicks* timebase, base::TimeDelta* interval);

  
  
  
  explicit RenderWidgetHostViewMac(RenderWidgetHost* widget);

  
  bool IsPopup() const;

  
  
  void ShutdownHost();

  bool CreateCompositedIOSurface();
  bool CreateCompositedIOSurfaceLayer();
  enum DestroyContextBehavior {
    kLeaveContextBoundToView,
    kDestroyContext,
  };
  void DestroyCompositedIOSurfaceAndLayer(DestroyContextBehavior
      destroy_context_behavior);

  
  void ClearBoundContextDrawable();

  
  void GotAcceleratedFrame();

  
  void GotSoftwareFrame();

  
  
  void AckPendingSwapBuffers();

  
  
  void ThrottledAckPendingSwapBuffers();

  void OnPluginFocusChanged(bool focused, int plugin_id);
  void OnStartPluginIme();
  CONTENT_EXPORT void OnAcceleratedSurfaceSetIOSurface(
      gfx::PluginWindowHandle window,
      int32 width,
      int32 height,
      uint64 mach_port);
  void OnAcceleratedSurfaceSetTransportDIB(gfx::PluginWindowHandle window,
                                           int32 width,
                                           int32 height,
                                           TransportDIB::Handle transport_dib);
  void OnAcceleratedSurfaceBuffersSwapped(gfx::PluginWindowHandle window,
                                          uint64 surface_handle);

  
  
  gfx::Rect GetScaledOpenGLPixelRect(const gfx::Rect& rect);

  
  
  
  RenderWidgetHostViewCocoa* cocoa_view_;

  
  bool is_loading_;

  
  base::string16 tooltip_text_;

  
  base::WeakPtrFactory<RenderWidgetHostViewMac> weak_factory_;

  
  std::string selected_text_;

  
  base::scoped_nsobject<NSWindow> popup_window_;

  
  base::scoped_nsobject<NSWindow> pepper_fullscreen_window_;
  base::scoped_nsobject<FullscreenWindowManager> fullscreen_window_manager_;
  
  RenderWidgetHostViewMac* fullscreen_parent_host_view_;

  
  
  std::list<std::pair<int32, int32> > pending_swap_buffers_acks_;

  
  base::WeakPtrFactory<RenderWidgetHostViewMac>
      pending_swap_buffers_acks_weak_factory_;

  
  
  
  base::WeakPtr<RenderWidgetHostViewMac> overlay_view_;

  
  gfx::Point overlay_view_offset_;

  
  
  
  base::WeakPtr<RenderWidgetHostViewMac> underlay_view_;

  
  
  bool underlay_view_has_drawn_;

  
  base::WeakPtrFactory<RenderWidgetHostViewMac>
      overlay_view_weak_factory_;

  
  
  
  base::Time next_swap_ack_time_;

  
  gfx::Range composition_range_;
  std::vector<gfx::Rect> composition_bounds_;

  
  gfx::Rect caret_rect_;

  
  scoped_ptr<RenderWidgetHostViewFrameSubscriber> frame_subscriber_;

  base::WeakPtrFactory<RenderWidgetHostViewMac>
      software_frame_weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewMac);
};

}  

#endif  
