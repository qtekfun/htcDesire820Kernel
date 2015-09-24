// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TREES_LAYER_TREE_HOST_IMPL_H_
#define CC_TREES_LAYER_TREE_HOST_IMPL_H_

#include <list>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "cc/animation/animation_events.h"
#include "cc/animation/animation_registrar.h"
#include "cc/base/cc_export.h"
#include "cc/debug/micro_benchmark_controller_impl.h"
#include "cc/input/input_handler.h"
#include "cc/input/layer_scroll_offset_delegate.h"
#include "cc/input/top_controls_manager_client.h"
#include "cc/layers/layer_lists.h"
#include "cc/layers/render_pass_sink.h"
#include "cc/output/begin_frame_args.h"
#include "cc/output/managed_memory_policy.h"
#include "cc/output/output_surface_client.h"
#include "cc/output/renderer.h"
#include "cc/quads/render_pass.h"
#include "cc/resources/resource_provider.h"
#include "cc/resources/tile_manager.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/rect.h"

namespace cc {

class CompletionEvent;
class CompositorFrameMetadata;
class DebugRectHistory;
class FrameRateCounter;
class LayerImpl;
class LayerTreeHostImplTimeSourceAdapter;
class LayerTreeImpl;
class PageScaleAnimation;
class PaintTimeCounter;
class MemoryHistory;
class RenderingStatsInstrumentation;
class RenderPassDrawQuad;
class ScrollbarLayerImplBase;
class TextureMailboxDeleter;
class TopControlsManager;
class UIResourceBitmap;
class UIResourceRequest;
struct RendererCapabilitiesImpl;

class LayerTreeHostImplClient {
 public:
  virtual void UpdateRendererCapabilitiesOnImplThread() = 0;
  virtual void DidLoseOutputSurfaceOnImplThread() = 0;
  virtual void DidSwapBuffersOnImplThread() = 0;
  virtual void OnSwapBuffersCompleteOnImplThread() = 0;
  virtual void BeginImplFrame(const BeginFrameArgs& args) = 0;
  virtual void OnCanDrawStateChanged(bool can_draw) = 0;
  virtual void NotifyReadyToActivate() = 0;
  
  
  virtual void SetNeedsRedrawOnImplThread() = 0;
  virtual void SetNeedsRedrawRectOnImplThread(gfx::Rect damage_rect) = 0;
  virtual void DidInitializeVisibleTileOnImplThread() = 0;
  virtual void SetNeedsCommitOnImplThread() = 0;
  virtual void SetNeedsManageTilesOnImplThread() = 0;
  virtual void PostAnimationEventsToMainThreadOnImplThread(
      scoped_ptr<AnimationEventsVector> events,
      base::Time wall_clock_time) = 0;
  
  virtual bool ReduceContentsTextureMemoryOnImplThread(
      size_t limit_bytes,
      int priority_cutoff) = 0;
  virtual void SendManagedMemoryStats() = 0;
  virtual bool IsInsideDraw() = 0;
  virtual void RenewTreePriority() = 0;
  virtual void RequestScrollbarAnimationOnImplThread(base::TimeDelta delay) = 0;
  virtual void DidActivatePendingTree() = 0;
  virtual void DidManageTiles() = 0;

 protected:
  virtual ~LayerTreeHostImplClient() {}
};

class CC_EXPORT LayerTreeHostImpl
    : public InputHandler,
      public RendererClient,
      public TileManagerClient,
      public OutputSurfaceClient,
      public TopControlsManagerClient,
      public base::SupportsWeakPtr<LayerTreeHostImpl> {
 public:
  static scoped_ptr<LayerTreeHostImpl> Create(
      const LayerTreeSettings& settings,
      LayerTreeHostImplClient* client,
      Proxy* proxy,
      RenderingStatsInstrumentation* rendering_stats_instrumentation,
      SharedBitmapManager* manager,
      int id);
  virtual ~LayerTreeHostImpl();

  
  virtual void BindToClient(InputHandlerClient* client) OVERRIDE;
  virtual InputHandler::ScrollStatus ScrollBegin(
      gfx::Point viewport_point,
      InputHandler::ScrollInputType type) OVERRIDE;
  virtual bool ScrollBy(gfx::Point viewport_point,
                        gfx::Vector2dF scroll_delta) OVERRIDE;
  virtual bool ScrollVerticallyByPage(gfx::Point viewport_point,
                                      ScrollDirection direction) OVERRIDE;
  virtual void SetRootLayerScrollOffsetDelegate(
      LayerScrollOffsetDelegate* root_layer_scroll_offset_delegate) OVERRIDE;
  virtual void OnRootLayerDelegatedScrollOffsetChanged() OVERRIDE;
  virtual void ScrollEnd() OVERRIDE;
  virtual InputHandler::ScrollStatus FlingScrollBegin() OVERRIDE;
  virtual void NotifyCurrentFlingVelocity(gfx::Vector2dF velocity) OVERRIDE;
  virtual void MouseMoveAt(gfx::Point viewport_point) OVERRIDE;
  virtual void PinchGestureBegin() OVERRIDE;
  virtual void PinchGestureUpdate(float magnify_delta,
                                  gfx::Point anchor) OVERRIDE;
  virtual void PinchGestureEnd() OVERRIDE;
  virtual void StartPageScaleAnimation(gfx::Vector2d target_offset,
                                       bool anchor_point,
                                       float page_scale,
                                       base::TimeDelta duration) OVERRIDE;
  virtual void ScheduleAnimation() OVERRIDE;
  virtual bool HaveTouchEventHandlersAt(gfx::Point viewport_port) OVERRIDE;
  virtual scoped_ptr<SwapPromiseMonitor> CreateLatencyInfoSwapPromiseMonitor(
      ui::LatencyInfo* latency) OVERRIDE;

  
  virtual void DidChangeTopControlsPosition() OVERRIDE;
  virtual bool HaveRootScrollLayer() const OVERRIDE;

  void StartScrollbarAnimation();

  struct CC_EXPORT FrameData : public RenderPassSink {
    FrameData();
    virtual ~FrameData();
    scoped_ptr<base::Value> AsValue() const;

    std::vector<gfx::Rect> occluding_screen_space_rects;
    std::vector<gfx::Rect> non_occluding_screen_space_rects;
    RenderPassList render_passes;
    RenderPassIdHashMap render_passes_by_id;
    const LayerImplList* render_surface_layer_list;
    LayerImplList will_draw_layers;
    bool contains_incomplete_tile;
    bool has_no_damage;

    
    virtual void AppendRenderPass(scoped_ptr<RenderPass> render_pass) OVERRIDE;
  };

  virtual void BeginMainFrameAborted(bool did_handle);
  virtual void BeginCommit();
  virtual void CommitComplete();
  virtual void Animate(base::TimeTicks monotonic_time,
                       base::Time wall_clock_time);
  virtual void UpdateAnimationState(bool start_ready_animations);
  void MainThreadHasStoppedFlinging();
  void UpdateBackgroundAnimateTicking(bool should_background_tick);
  void DidAnimateScrollOffset();
  void SetViewportDamage(gfx::Rect damage_rect);

  virtual void ManageTiles();

  
  
  
  
  virtual bool PrepareToDraw(FrameData* frame,
                             gfx::Rect device_viewport_damage_rect);
  virtual void DrawLayers(FrameData* frame, base::TimeTicks frame_begin_time);
  
  void DidDrawAllLayers(const FrameData& frame);

  const LayerTreeSettings& settings() const { return settings_; }

  
  void EvictTexturesForTesting();

  
  
  
  virtual void BlockNotifyReadyToActivateForTesting(bool block);

  
  void DidInitializeVisibleTileForTesting();

  bool device_viewport_valid_for_tile_management() const {
    return device_viewport_valid_for_tile_management_;
  }

  
  
  gfx::Size DrawViewportSize() const;

  
  
  
  gfx::SizeF UnscaledScrollableViewportSize() const;

  
  virtual void SetFullRootLayerDamage() OVERRIDE;

  
  virtual void NotifyReadyToActivate() OVERRIDE;

  
  virtual bool DeferredInitialize(
      scoped_refptr<ContextProvider> offscreen_context_provider) OVERRIDE;
  virtual void ReleaseGL() OVERRIDE;
  virtual void SetNeedsRedrawRect(gfx::Rect rect) OVERRIDE;
  virtual void BeginImplFrame(const BeginFrameArgs& args) OVERRIDE;
  virtual void SetExternalDrawConstraints(
      const gfx::Transform& transform,
      gfx::Rect viewport,
      gfx::Rect clip,
      bool valid_for_tile_management) OVERRIDE;
  virtual void DidLoseOutputSurface() OVERRIDE;
  virtual void DidSwapBuffers() OVERRIDE;
  virtual void OnSwapBuffersComplete() OVERRIDE;
  virtual void ReclaimResources(const CompositorFrameAck* ack) OVERRIDE;
  virtual void SetMemoryPolicy(const ManagedMemoryPolicy& policy) OVERRIDE;
  virtual void SetTreeActivationCallback(const base::Closure& callback)
      OVERRIDE;

  
  void OnCanDrawStateChangedForTree();

  
  bool CanDraw() const;
  OutputSurface* output_surface() const { return output_surface_.get(); }

  void SetOffscreenContextProvider(
      const scoped_refptr<ContextProvider>& offscreen_context_provider);
  ContextProvider* offscreen_context_provider() const {
    return offscreen_context_provider_.get();
  }

  std::string LayerTreeAsJson() const;

  void FinishAllRendering();
  int SourceAnimationFrameNumber() const;

  virtual bool InitializeRenderer(scoped_ptr<OutputSurface> output_surface);
  bool IsContextLost();
  TileManager* tile_manager() { return tile_manager_.get(); }
  Renderer* renderer() { return renderer_.get(); }
  const RendererCapabilitiesImpl& GetRendererCapabilities() const;

  virtual bool SwapBuffers(const FrameData& frame);
  void SetNeedsBeginImplFrame(bool enable);
  void DidModifyTilePriorities();

  void Readback(void* pixels, gfx::Rect rect_in_device_viewport);

  LayerTreeImpl* active_tree() { return active_tree_.get(); }
  const LayerTreeImpl* active_tree() const { return active_tree_.get(); }
  LayerTreeImpl* pending_tree() { return pending_tree_.get(); }
  const LayerTreeImpl* pending_tree() const { return pending_tree_.get(); }
  const LayerTreeImpl* recycle_tree() const { return recycle_tree_.get(); }
  virtual void CreatePendingTree();
  virtual void UpdateVisibleTiles();
  virtual void ActivatePendingTree();

  
  LayerImpl* RootLayer() const;
  LayerImpl* RootScrollLayer() const;
  LayerImpl* CurrentlyScrollingLayer() const;

  int scroll_layer_id_when_mouse_over_scrollbar() {
    return scroll_layer_id_when_mouse_over_scrollbar_;
  }

  bool IsCurrentlyScrolling() const;

  virtual void SetVisible(bool visible);
  bool visible() const { return visible_; }

  void SetNeedsCommit() { client_->SetNeedsCommitOnImplThread(); }
  void SetNeedsRedraw();

  ManagedMemoryPolicy ActualManagedMemoryPolicy() const;

  size_t memory_allocation_limit_bytes() const;
  int memory_allocation_priority_cutoff() const;

  void SetViewportSize(gfx::Size device_viewport_size);

  void SetOverdrawBottomHeight(float overdraw_bottom_height);
  float overdraw_bottom_height() const { return overdraw_bottom_height_; }

  void SetOverhangUIResource(UIResourceId overhang_ui_resource_id,
                             gfx::Size overhang_ui_resource_size);

  void SetDeviceScaleFactor(float device_scale_factor);
  float device_scale_factor() const { return device_scale_factor_; }

  const gfx::Transform& DrawTransform() const;

  scoped_ptr<ScrollAndScaleSet> ProcessScrollDeltas();

  bool needs_animate_layers() const {
    return !animation_registrar_->active_animation_controllers().empty();
  }

  void SendManagedMemoryStats(
      size_t memory_visible_bytes,
      size_t memory_visible_and_nearby_bytes,
      size_t memory_use_bytes);

  void set_max_memory_needed_bytes(size_t bytes) {
    max_memory_needed_bytes_ = bytes;
  }

  FrameRateCounter* fps_counter() {
    return fps_counter_.get();
  }
  PaintTimeCounter* paint_time_counter() {
    return paint_time_counter_.get();
  }
  MemoryHistory* memory_history() {
    return memory_history_.get();
  }
  DebugRectHistory* debug_rect_history() {
    return debug_rect_history_.get();
  }
  ResourceProvider* resource_provider() {
    return resource_provider_.get();
  }
  TopControlsManager* top_controls_manager() {
    return top_controls_manager_.get();
  }

  Proxy* proxy() const { return proxy_; }

  AnimationRegistrar* animation_registrar() const {
    return animation_registrar_.get();
  }

  void SetDebugState(const LayerTreeDebugState& new_debug_state);
  const LayerTreeDebugState& debug_state() const { return debug_state_; }

  class CC_EXPORT CullRenderPassesWithNoQuads {
 public:
    bool ShouldRemoveRenderPass(const RenderPassDrawQuad& quad,
                                const FrameData& frame) const;

    
    
    size_t RenderPassListBegin(const RenderPassList& list) const { return 0; }
    size_t RenderPassListEnd(const RenderPassList& list) const {
      return list.size();
    }
    size_t RenderPassListNext(size_t it) const { return it + 1; }
  };

  template <typename RenderPassCuller>
      static void RemoveRenderPasses(RenderPassCuller culler, FrameData* frame);

  gfx::Vector2dF accumulated_root_overscroll() const {
    return accumulated_root_overscroll_;
  }
  gfx::Vector2dF current_fling_velocity() const {
    return current_fling_velocity_;
  }

  bool pinch_gesture_active() const { return pinch_gesture_active_; }

  void SetTreePriority(TreePriority priority);

  void ResetCurrentFrameTimeForNextFrame();
  virtual base::TimeTicks CurrentFrameTimeTicks();
  base::Time CurrentFrameTime();

  virtual base::TimeTicks CurrentPhysicalTimeTicks() const;

  scoped_ptr<base::Value> AsValue() const { return AsValueWithFrame(NULL); }
  scoped_ptr<base::Value> AsValueWithFrame(FrameData* frame) const;
  scoped_ptr<base::Value> ActivationStateAsValue() const;

  bool page_scale_animation_active() const { return !!page_scale_animation_; }

  virtual void CreateUIResource(UIResourceId uid,
                                const UIResourceBitmap& bitmap);
  
  virtual void DeleteUIResource(UIResourceId uid);
  void EvictAllUIResources();
  bool EvictedUIResourcesExist() const;

  virtual ResourceProvider::ResourceId ResourceIdForUIResource(
      UIResourceId uid) const;

  virtual bool IsUIResourceOpaque(UIResourceId uid) const;

  struct UIResourceData {
    ResourceProvider::ResourceId resource_id;
    gfx::Size size;
    bool opaque;
  };

  void ScheduleMicroBenchmark(scoped_ptr<MicroBenchmarkImpl> benchmark);

  CompositorFrameMetadata MakeCompositorFrameMetadata() const;
  
  
  
  gfx::Rect DeviceViewport() const;
  gfx::Rect DeviceClip() const;

  
  
  
  
  void InsertSwapPromiseMonitor(SwapPromiseMonitor* monitor);
  void RemoveSwapPromiseMonitor(SwapPromiseMonitor* monitor);

 protected:
  LayerTreeHostImpl(
      const LayerTreeSettings& settings,
      LayerTreeHostImplClient* client,
      Proxy* proxy,
      RenderingStatsInstrumentation* rendering_stats_instrumentation,
      SharedBitmapManager* manager,
      int id);

  
  virtual void AnimateLayers(base::TimeTicks monotonic_time,
                             base::Time wall_clock_time);

  
  virtual base::TimeDelta LowFrequencyAnimationInterval() const;

  const AnimationRegistrar::AnimationControllerMap&
      active_animation_controllers() const {
    return animation_registrar_->active_animation_controllers();
  }

  bool manage_tiles_needed() const { return tile_priorities_dirty_; }

  LayerTreeHostImplClient* client_;
  Proxy* proxy_;

 private:
  void CreateAndSetRenderer(
      OutputSurface* output_surface,
      ResourceProvider* resource_provider,
      bool skip_gl_renderer);
  void CreateAndSetTileManager(ResourceProvider* resource_provider,
                               ContextProvider* context_provider,
                               bool using_map_image);
  void ReleaseTreeResources();
  void EnforceZeroBudget(bool zero_budget);

  void AnimatePageScale(base::TimeTicks monotonic_time);
  void AnimateScrollbars(base::TimeTicks monotonic_time);
  void AnimateTopControls(base::TimeTicks monotonic_time);

  gfx::Vector2dF ScrollLayerWithViewportSpaceDelta(
      LayerImpl* layer_impl,
      float scale_from_viewport_to_screen_space,
      gfx::PointF viewport_point,
      gfx::Vector2dF viewport_delta);

  void UpdateMaxScrollOffset();
  void TrackDamageForAllSurfaces(
      LayerImpl* root_draw_layer,
      const LayerImplList& render_surface_layer_list);

  void UpdateTileManagerMemoryPolicy(const ManagedMemoryPolicy& policy);

  
  
  
  bool CalculateRenderPasses(FrameData* frame);

  void SendReleaseResourcesRecursive(LayerImpl* current);
  bool EnsureRenderSurfaceLayerList();
  void ClearCurrentlyScrollingLayer();

  bool HandleMouseOverScrollbar(LayerImpl* layer_impl,
                                gfx::PointF device_viewport_point);

  void AnimateScrollbarsRecursive(LayerImpl* layer,
                                  base::TimeTicks time);

  void UpdateCurrentFrameTime(base::TimeTicks* ticks, base::Time* now) const;

  LayerImpl* FindScrollLayerForDeviceViewportPoint(
      gfx::PointF device_viewport_point,
      InputHandler::ScrollInputType type,
      LayerImpl* layer_hit_by_point,
      bool* scroll_on_main_thread) const;
  float DeviceSpaceDistanceToLayer(gfx::PointF device_viewport_point,
                                   LayerImpl* layer_impl);
  void StartScrollbarAnimationRecursive(LayerImpl* layer, base::TimeTicks time);
  void SetManagedMemoryPolicy(const ManagedMemoryPolicy& policy,
                              bool zero_budget);
  void EnforceManagedMemoryPolicy(const ManagedMemoryPolicy& policy);

  void DidInitializeVisibleTile();

  void MarkUIResourceNotEvicted(UIResourceId uid);

  void NotifySwapPromiseMonitorsOfSetNeedsRedraw();

  typedef base::hash_map<UIResourceId, UIResourceData>
      UIResourceMap;
  UIResourceMap ui_resource_map_;

  
  
  
  std::set<UIResourceId> evicted_ui_resources_;

  scoped_ptr<OutputSurface> output_surface_;
  scoped_refptr<ContextProvider> offscreen_context_provider_;

  
  
  scoped_ptr<ResourceProvider> resource_provider_;
  scoped_ptr<TileManager> tile_manager_;
  scoped_ptr<Renderer> renderer_;

  GlobalStateThatImpactsTilePriority global_tile_state_;

  
  scoped_ptr<LayerTreeImpl> active_tree_;

  
  
  scoped_ptr<LayerTreeImpl> pending_tree_;

  
  
  scoped_ptr<LayerTreeImpl> recycle_tree_;

  InputHandlerClient* input_handler_client_;
  bool did_lock_scrolling_layer_;
  bool should_bubble_scrolls_;
  bool last_scroll_did_bubble_;
  bool wheel_scrolling_;
  int scroll_layer_id_when_mouse_over_scrollbar_;

  bool tile_priorities_dirty_;

  
  LayerScrollOffsetDelegate* root_layer_scroll_offset_delegate_;
  LayerTreeSettings settings_;
  LayerTreeDebugState debug_state_;
  bool visible_;
  ManagedMemoryPolicy cached_managed_memory_policy_;

  gfx::Vector2dF accumulated_root_overscroll_;
  gfx::Vector2dF current_fling_velocity_;

  bool pinch_gesture_active_;
  bool pinch_gesture_end_should_clear_scrolling_layer_;
  gfx::Point previous_pinch_anchor_;

  
  
  base::Time last_animation_time_;

  scoped_ptr<TopControlsManager> top_controls_manager_;

  scoped_ptr<PageScaleAnimation> page_scale_animation_;

  
  scoped_ptr<LayerTreeHostImplTimeSourceAdapter> time_source_client_adapter_;

  scoped_ptr<FrameRateCounter> fps_counter_;
  scoped_ptr<PaintTimeCounter> paint_time_counter_;
  scoped_ptr<MemoryHistory> memory_history_;
  scoped_ptr<DebugRectHistory> debug_rect_history_;

  scoped_ptr<TextureMailboxDeleter> texture_mailbox_deleter_;

  
  
  size_t max_memory_needed_bytes_;

  size_t last_sent_memory_visible_bytes_;
  size_t last_sent_memory_visible_and_nearby_bytes_;
  size_t last_sent_memory_use_bytes_;
  bool zero_budget_;

  
  
  
  
  gfx::Size device_viewport_size_;

  
  
  float device_scale_factor_;

  
  UIResourceId overhang_ui_resource_id_;
  gfx::Size overhang_ui_resource_size_;

  
  
  
  
  float overdraw_bottom_height_;

  
  
  
  
  
  
  
  
  gfx::Transform external_transform_;
  gfx::Rect external_viewport_;
  gfx::Rect external_clip_;
  bool device_viewport_valid_for_tile_management_;
  bool external_stencil_test_enabled_;

  gfx::Rect viewport_damage_rect_;

  base::TimeTicks current_frame_timeticks_;
  base::Time current_frame_time_;

  scoped_ptr<AnimationRegistrar> animation_registrar_;

  RenderingStatsInstrumentation* rendering_stats_instrumentation_;
  MicroBenchmarkControllerImpl micro_benchmark_controller_;

  bool need_to_update_visible_tiles_before_draw_;
#ifndef NDEBUG
  bool did_lose_called_;
#endif

  
  base::Closure tree_activation_callback_;

  SharedBitmapManager* shared_bitmap_manager_;
  int id_;

  std::set<SwapPromiseMonitor*> swap_promise_monitor_;

  DISALLOW_COPY_AND_ASSIGN(LayerTreeHostImpl);
};

}  

#endif  
