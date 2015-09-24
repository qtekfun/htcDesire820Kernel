// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_OUTPUT_SURFACE_H_
#define CC_OUTPUT_OUTPUT_SURFACE_H_

#include <deque>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "cc/base/cc_export.h"
#include "cc/output/context_provider.h"
#include "cc/output/software_output_device.h"
#include "cc/scheduler/frame_rate_controller.h"
#include "cc/scheduler/rolling_time_delta_history.h"

namespace base { class SingleThreadTaskRunner; }

namespace ui { struct LatencyInfo; }

namespace gfx {
class Rect;
class Size;
class Transform;
}

namespace cc {

class CompositorFrame;
class CompositorFrameAck;
struct ManagedMemoryPolicy;
class OutputSurfaceClient;

class CC_EXPORT OutputSurface : public FrameRateControllerClient {
 public:
  enum {
    DEFAULT_MAX_FRAMES_PENDING = 2
  };

  explicit OutputSurface(scoped_refptr<ContextProvider> context_provider);

  explicit OutputSurface(scoped_ptr<SoftwareOutputDevice> software_device);

  OutputSurface(scoped_refptr<ContextProvider> context_provider,
                scoped_ptr<SoftwareOutputDevice> software_device);

  virtual ~OutputSurface();

  struct Capabilities {
    Capabilities()
        : delegated_rendering(false),
          max_frames_pending(0),
          deferred_gl_initialization(false),
          draw_and_swap_full_viewport_every_frame(false),
          adjust_deadline_for_parent(true),
          uses_default_gl_framebuffer(true) {}
    bool delegated_rendering;
    int max_frames_pending;
    bool deferred_gl_initialization;
    bool draw_and_swap_full_viewport_every_frame;
    
    
    bool adjust_deadline_for_parent;
    
    
    bool uses_default_gl_framebuffer;
  };

  const Capabilities& capabilities() const {
    return capabilities_;
  }

  virtual bool HasExternalStencilTest() const;

  
  
  
  
  scoped_refptr<ContextProvider> context_provider() const {
    return context_provider_.get();
  }
  SoftwareOutputDevice* software_device() const {
    return software_device_.get();
  }

  
  
  
  virtual bool ForcedDrawToSoftwareDevice() const;

  
  
  
  
  virtual bool BindToClient(OutputSurfaceClient* client);

  void InitializeBeginImplFrameEmulation(
      base::SingleThreadTaskRunner* task_runner,
      bool throttle_frame_production,
      base::TimeDelta interval);

  void SetMaxFramesPending(int max_frames_pending);

  virtual void EnsureBackbuffer();
  virtual void DiscardBackbuffer();

  virtual void Reshape(gfx::Size size, float scale_factor);
  virtual gfx::Size SurfaceSize() const;

  virtual void BindFramebuffer();

  
  
  
  virtual void SwapBuffers(CompositorFrame* frame);

  
  
  virtual void UpdateSmoothnessTakesPriority(bool prefer_smoothness) {}

  
  
  
  virtual void SetNeedsBeginImplFrame(bool enable);

  bool HasClient() { return !!client_; }

  
  
  base::TimeDelta GpuLatencyEstimate();

 protected:
  
  
  
  
  bool InitializeAndSetContext3d(
      scoped_refptr<ContextProvider> context_provider,
      scoped_refptr<ContextProvider> offscreen_context_provider);
  void ReleaseGL();

  void PostSwapBuffersComplete();

  struct OutputSurface::Capabilities capabilities_;
  scoped_refptr<ContextProvider> context_provider_;
  scoped_ptr<SoftwareOutputDevice> software_device_;
  gfx::Size surface_size_;
  float device_scale_factor_;

  
  
  void OnVSyncParametersChanged(base::TimeTicks timebase,
                                base::TimeDelta interval);
  virtual void FrameRateControllerTick(bool throttled,
                                       const BeginFrameArgs& args) OVERRIDE;
  scoped_ptr<FrameRateController> frame_rate_controller_;
  int max_frames_pending_;
  int pending_swap_buffers_;
  bool needs_begin_impl_frame_;
  bool client_ready_for_begin_impl_frame_;

  
  
  BeginFrameArgs skipped_begin_impl_frame_args_;

  
  
  void SetNeedsRedrawRect(gfx::Rect damage_rect);
  void BeginImplFrame(const BeginFrameArgs& args);
  void DidSwapBuffers();
  void OnSwapBuffersComplete();
  void ReclaimResources(const CompositorFrameAck* ack);
  void DidLoseOutputSurface();
  void SetExternalStencilTest(bool enabled);
  void SetExternalDrawConstraints(const gfx::Transform& transform,
                                  gfx::Rect viewport,
                                  gfx::Rect clip,
                                  bool valid_for_tile_management);

  
  virtual base::TimeTicks RetroactiveBeginImplFrameDeadline();
  virtual void PostCheckForRetroactiveBeginImplFrame();
  void CheckForRetroactiveBeginImplFrame();

 private:
  OutputSurfaceClient* client_;

  void SetUpContext3d();
  void ResetContext3d();
  void SetMemoryPolicy(const ManagedMemoryPolicy& policy);
  void UpdateAndMeasureGpuLatency();

  
  
  bool check_for_retroactive_begin_impl_frame_pending_;

  bool external_stencil_test_enabled_;

  base::WeakPtrFactory<OutputSurface> weak_ptr_factory_;

  std::deque<unsigned> available_gpu_latency_query_ids_;
  std::deque<unsigned> pending_gpu_latency_query_ids_;
  RollingTimeDeltaHistory gpu_latency_history_;

  DISALLOW_COPY_AND_ASSIGN(OutputSurface);
};

}  

#endif  
