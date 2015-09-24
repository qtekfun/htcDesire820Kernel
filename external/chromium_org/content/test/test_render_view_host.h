// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_TEST_RENDER_VIEW_HOST_H_
#define CONTENT_TEST_TEST_RENDER_VIEW_HOST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "build/build_config.h"
#include "content/browser/renderer_host/render_view_host_impl.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/test/test_renderer_host.h"
#include "content/test/test_render_frame_host.h"
#include "ui/base/layout.h"
#include "ui/gfx/vector2d_f.h"


struct ViewHostMsg_FrameNavigate_Params;

namespace gfx {
class Rect;
}

namespace content {

class SiteInstance;
class TestWebContents;

void InitNavigateParams(ViewHostMsg_FrameNavigate_Params* params,
                        int page_id,
                        const GURL& url,
                        PageTransition transition_type);


class TestRenderWidgetHostView : public RenderWidgetHostViewBase {
 public:
  explicit TestRenderWidgetHostView(RenderWidgetHost* rwh);
  virtual ~TestRenderWidgetHostView();

  
  virtual void InitAsChild(gfx::NativeView parent_view) OVERRIDE {}
  virtual RenderWidgetHost* GetRenderWidgetHost() const OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE {}
  virtual void SetBounds(const gfx::Rect& rect) OVERRIDE {}
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeViewId GetNativeViewId() const OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;
  virtual bool HasFocus() const OVERRIDE;
  virtual bool IsSurfaceAvailableForCopy() const OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsShowing() OVERRIDE;
  virtual gfx::Rect GetViewBounds() const OVERRIDE;
#if defined(OS_MACOSX)
  virtual void SetActive(bool active) OVERRIDE;
  virtual void SetTakesFocusOnlyOnMouseDown(bool flag) OVERRIDE {}
  virtual void SetWindowVisibility(bool visible) OVERRIDE {}
  virtual void WindowFrameChanged() OVERRIDE {}
  virtual void ShowDefinitionForSelection() OVERRIDE {}
  virtual bool SupportsSpeech() const OVERRIDE;
  virtual void SpeakSelection() OVERRIDE;
  virtual bool IsSpeaking() const OVERRIDE;
  virtual void StopSpeaking() OVERRIDE;
#endif  
#if defined(TOOLKIT_GTK)
  virtual GdkEventButton* GetLastMouseDown() OVERRIDE;
  virtual gfx::NativeView BuildInputMethodsGtkMenu() OVERRIDE;
#endif  
  virtual void OnSwapCompositorFrame(
      uint32 output_surface_id,
      scoped_ptr<cc::CompositorFrame> frame) OVERRIDE;

  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos) OVERRIDE {}
  virtual void InitAsFullscreen(
      RenderWidgetHostView* reference_host_view) OVERRIDE {}
  virtual void WasShown() OVERRIDE {}
  virtual void WasHidden() OVERRIDE {}
  virtual void MovePluginWindows(
      const gfx::Vector2d& scroll_offset,
      const std::vector<WebPluginGeometry>& moves) OVERRIDE {}
  virtual void Focus() OVERRIDE {}
  virtual void Blur() OVERRIDE {}
  virtual void SetIsLoading(bool is_loading) OVERRIDE {}
  virtual void UpdateCursor(const WebCursor& cursor) OVERRIDE {}
  virtual void TextInputTypeChanged(ui::TextInputType type,
                                    ui::TextInputMode input_mode,
                                    bool can_compose_inline) OVERRIDE {}
  virtual void ImeCancelComposition() OVERRIDE {}
#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  virtual void ImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds) OVERRIDE {}
#endif
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect,
      const gfx::Vector2d& scroll_delta,
      const std::vector<gfx::Rect>& rects,
      const ui::LatencyInfo& latency_info) OVERRIDE {}
  virtual void RenderProcessGone(base::TerminationStatus status,
                                 int error_code) OVERRIDE;
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh) { }
  virtual void Destroy() OVERRIDE;
  virtual void SetTooltipText(const base::string16& tooltip_text) OVERRIDE {}
  virtual void SelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params) OVERRIDE {}
  virtual void ScrollOffsetChanged() OVERRIDE {}
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
  virtual void AcceleratedSurfaceRelease() OVERRIDE {}
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) OVERRIDE;
#if defined(OS_MACOSX)
  virtual void AboutToWaitForBackingStoreMsg() OVERRIDE;
  virtual bool PostProcessEventForPluginIme(
      const NativeWebKeyboardEvent& event) OVERRIDE;
#elif defined(OS_ANDROID)
  virtual void ShowDisambiguationPopup(
      const gfx::Rect& target_rect,
      const SkBitmap& zoomed_bitmap) OVERRIDE {}
  virtual void HasTouchEventHandlers(bool need_touch_events) OVERRIDE {}
#elif defined(OS_WIN) && !defined(USE_AURA)
  virtual void WillWmDestroy() OVERRIDE;
#endif
  virtual void GetScreenInfo(blink::WebScreenInfo* results) OVERRIDE {}
  virtual gfx::Rect GetBoundsInRootWindow() OVERRIDE;
  virtual void SetHasHorizontalScrollbar(
      bool has_horizontal_scrollbar) OVERRIDE { }
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) OVERRIDE { }
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params) OVERRIDE {}
  virtual gfx::GLSurfaceHandle GetCompositingSurface() OVERRIDE;
#if defined(OS_WIN) && !defined(USE_AURA)
  virtual void SetClickthroughRegion(SkRegion* region) OVERRIDE;
#endif
  virtual bool LockMouse() OVERRIDE;
  virtual void UnlockMouse() OVERRIDE;
#if defined(OS_WIN) && defined(USE_AURA)
  virtual void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent) OVERRIDE;
  virtual gfx::NativeViewId GetParentForWindowlessPlugin() const OVERRIDE;
#endif

  bool is_showing() const { return is_showing_; }
  bool did_swap_compositor_frame() const { return did_swap_compositor_frame_; }

 protected:
  RenderWidgetHostImpl* rwh_;

 private:
  bool is_showing_;
  bool did_swap_compositor_frame_;
};

#if defined(COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4250)
#endif


class TestRenderViewHost
    : public RenderViewHostImpl,
      public RenderViewHostTester {
 public:
  TestRenderViewHost(SiteInstance* instance,
                     RenderViewHostDelegate* delegate,
                     RenderFrameHostDelegate* frame_delegate,
                     RenderWidgetHostDelegate* widget_delegate,
                     int routing_id,
                     int main_frame_routing_id,
                     bool swapped_out);
  virtual ~TestRenderViewHost();

  
  
  
  virtual void SendNavigate(int page_id, const GURL& url) OVERRIDE;
  virtual void SendFailedNavigate(int page_id, const GURL& url) OVERRIDE;
  virtual void SendNavigateWithTransition(int page_id, const GURL& url,
                                          PageTransition transition) OVERRIDE;
  virtual void SendShouldCloseACK(bool proceed) OVERRIDE;
  virtual void SetContentsMimeType(const std::string& mime_type) OVERRIDE;
  virtual void SimulateSwapOutACK() OVERRIDE;
  virtual void SimulateWasHidden() OVERRIDE;
  virtual void SimulateWasShown() OVERRIDE;

  
  
  
  
  void SendNavigateWithOriginalRequestURL(
      int page_id, const GURL& url, const GURL& original_request_url);

  void SendNavigateWithFile(
      int page_id, const GURL& url, const base::FilePath& file_path);

  void SendNavigateWithParams(ViewHostMsg_FrameNavigate_Params* params);

  void TestOnUpdateStateWithFile(
      int process_id, const base::FilePath& file_path);

  void TestOnStartDragging(const DropData& drop_data);

  
  void set_delete_counter(int* delete_counter) {
    delete_counter_ = delete_counter;
  }

  
  
  
  void set_render_view_created(bool created) {
    render_view_created_ = created;
  }

  
  
  bool is_waiting_for_beforeunload_ack() const {
    return is_waiting_for_beforeunload_ack_;
  }

  
  
  bool is_waiting_for_unload_ack() const {
    return is_waiting_for_unload_ack_;
  }

  
  
  void set_is_swapped_out(bool is_swapped_out) {
    is_swapped_out_ = is_swapped_out;
  }

  
  
  
  void set_simulate_fetch_via_proxy(bool proxy);

  
  
  
  void set_simulate_history_list_was_cleared(bool cleared);

  
  int opener_route_id() const { return opener_route_id_; }

  

  virtual bool CreateRenderView(const base::string16& frame_name,
                                int opener_route_id,
                                int32 max_page_id) OVERRIDE;
  virtual bool IsRenderViewLive() const OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(RenderViewHostTest, FilterNavigate);

  void SendNavigateWithTransitionAndResponseCode(int page_id,
                                                 const GURL& url,
                                                 PageTransition transition,
                                                 int response_code);

  
  
  
  void SendNavigateWithParameters(
      int page_id,
      const GURL& url,
      PageTransition transition,
      const GURL& original_request_url,
      int response_code,
      const base::FilePath* file_path_for_history_item);

  
  
  bool render_view_created_;

  
  int* delete_counter_;

  
  bool simulate_fetch_via_proxy_;

  
  bool simulate_history_list_was_cleared_;

  
  std::string contents_mime_type_;

  
  int opener_route_id_;

  DISALLOW_COPY_AND_ASSIGN(TestRenderViewHost);
};

#if defined(COMPILER_MSVC)
#pragma warning(pop)
#endif

class RenderViewHostImplTestHarness : public RenderViewHostTestHarness {
 public:
  RenderViewHostImplTestHarness();
  virtual ~RenderViewHostImplTestHarness();

  TestRenderViewHost* test_rvh();
  TestRenderViewHost* pending_test_rvh();
  TestRenderViewHost* active_test_rvh();
  TestRenderFrameHost* main_test_rfh();
  TestWebContents* contents();

 private:
  typedef scoped_ptr<ui::test::ScopedSetSupportedScaleFactors>
      ScopedSetSupportedScaleFactors;
  ScopedSetSupportedScaleFactors scoped_set_supported_scale_factors_;
  DISALLOW_COPY_AND_ASSIGN(RenderViewHostImplTestHarness);
};

}  

#endif  
