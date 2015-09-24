// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_COMPOSITOR_H_
#define UI_COMPOSITOR_COMPOSITOR_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "cc/trees/layer_tree_host_client.h"
#include "cc/trees/layer_tree_host_single_thread_client.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/compositor/compositor_export.h"
#include "ui/compositor/compositor_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vector2d.h"

class SkBitmap;

namespace base {
class MessageLoopProxy;
class RunLoop;
}

namespace blink {
class WebGraphicsContext3D;
}

namespace cc {
class ContextProvider;
class Layer;
class LayerTreeDebugState;
class LayerTreeHost;
}

namespace gfx {
class Rect;
class Size;
}

namespace ui {

class Compositor;
class Layer;
class PostedSwapQueue;
class Reflector;
class Texture;
struct LatencyInfo;

class COMPOSITOR_EXPORT ContextFactory {
 public:
  virtual ~ContextFactory() {}

  
  static ContextFactory* GetInstance();

  
  
  
  static void SetInstance(ContextFactory* instance);

  
  
  
  virtual scoped_ptr<cc::OutputSurface> CreateOutputSurface(
      Compositor* compositor, bool software_fallback) = 0;

  
  
  virtual scoped_refptr<Reflector> CreateReflector(
      Compositor* mirrored_compositor,
      Layer* mirroring_layer) = 0;
  
  virtual void RemoveReflector(scoped_refptr<Reflector> reflector) = 0;

  
  
  
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenCompositorContextProvider() = 0;

  
  
  
  
  virtual scoped_refptr<cc::ContextProvider>
      SharedMainThreadContextProvider() = 0;

  
  virtual void RemoveCompositor(Compositor* compositor) = 0;

  
  
  virtual bool DoesCreateTestContexts() = 0;
};

class COMPOSITOR_EXPORT Texture : public base::RefCounted<Texture> {
 public:
  Texture(bool flipped, const gfx::Size& size, float device_scale_factor);

  bool flipped() const { return flipped_; }
  gfx::Size size() const { return size_; }
  float device_scale_factor() const { return device_scale_factor_; }

  virtual unsigned int PrepareTexture() = 0;

  
  virtual void Consume(const std::string& mailbox_name,
                       const gfx::Size& new_size) {}

  
  
  virtual std::string Produce();

 protected:
  virtual ~Texture();
  gfx::Size size_;  

 private:
  friend class base::RefCounted<Texture>;

  bool flipped_;
  float device_scale_factor_;

  DISALLOW_COPY_AND_ASSIGN(Texture);
};

class COMPOSITOR_EXPORT CompositorLock
    : public base::RefCounted<CompositorLock>,
      public base::SupportsWeakPtr<CompositorLock> {
 private:
  friend class base::RefCounted<CompositorLock>;
  friend class Compositor;

  explicit CompositorLock(Compositor* compositor);
  ~CompositorLock();

  void CancelLock();

  Compositor* compositor_;
  DISALLOW_COPY_AND_ASSIGN(CompositorLock);
};

class COMPOSITOR_EXPORT DrawWaiterForTest : public CompositorObserver {
 public:
  
  
  
  static void Wait(Compositor* compositor);

  
  static void WaitForCommit(Compositor* compositor);

 private:
  DrawWaiterForTest();
  virtual ~DrawWaiterForTest();

  void WaitImpl(Compositor* compositor);

  
  virtual void OnCompositingDidCommit(Compositor* compositor) OVERRIDE;
  virtual void OnCompositingStarted(Compositor* compositor,
                                    base::TimeTicks start_time) OVERRIDE;
  virtual void OnCompositingEnded(Compositor* compositor) OVERRIDE;
  virtual void OnCompositingAborted(Compositor* compositor) OVERRIDE;
  virtual void OnCompositingLockStateChanged(Compositor* compositor) OVERRIDE;
  virtual void OnUpdateVSyncParameters(Compositor* compositor,
                                       base::TimeTicks timebase,
                                       base::TimeDelta interval) OVERRIDE;

  scoped_ptr<base::RunLoop> wait_run_loop_;

  bool wait_for_commit_;

  DISALLOW_COPY_AND_ASSIGN(DrawWaiterForTest);
};

class COMPOSITOR_EXPORT Compositor
    : NON_EXPORTED_BASE(public cc::LayerTreeHostClient),
      NON_EXPORTED_BASE(public cc::LayerTreeHostSingleThreadClient),
      public base::SupportsWeakPtr<Compositor> {
 public:
  explicit Compositor(gfx::AcceleratedWidget widget);
  virtual ~Compositor();

  static void Initialize();
  static bool WasInitializedWithThread();
  static scoped_refptr<base::MessageLoopProxy> GetCompositorMessageLoop();
  static void Terminate();

  
  void ScheduleDraw();

  
  
  
  
  
  const Layer* root_layer() const { return root_layer_; }
  Layer* root_layer() { return root_layer_; }
  void SetRootLayer(Layer* root_layer);

  
  
  
  void SetHostHasTransparentBackground(bool host_has_transparent_background);

  
  
  float device_scale_factor() const { return device_scale_factor_; }

  
  void Draw();

  
  
  
  void ScheduleFullRedraw();

  
  
  void ScheduleRedrawRect(const gfx::Rect& damage_rect);

  void SetLatencyInfo(const LatencyInfo& latency_info);

  
  
  
  bool ReadPixels(SkBitmap* bitmap, const gfx::Rect& bounds_in_pixel);

  
  void SetScaleAndSize(float scale, const gfx::Size& size_in_pixel);

  
  const gfx::Size& size() const { return size_; }

  
  
  void SetBackgroundColor(SkColor color);

  
  gfx::AcceleratedWidget widget() const { return widget_; }

  
  
  void AddObserver(CompositorObserver* observer);
  void RemoveObserver(CompositorObserver* observer);
  bool HasObserver(CompositorObserver* observer);

  
  
  scoped_refptr<CompositorLock> GetCompositorLock();

  
  

  
  void OnSwapBuffersPosted();

  
  void OnSwapBuffersComplete();

  
  void OnSwapBuffersAborted();

  void OnUpdateVSyncParameters(base::TimeTicks timebase,
                               base::TimeDelta interval);

  
  virtual void WillBeginMainFrame(int frame_id) OVERRIDE {}
  virtual void DidBeginMainFrame() OVERRIDE {}
  virtual void Animate(double frame_begin_time) OVERRIDE {}
  virtual void Layout() OVERRIDE;
  virtual void ApplyScrollAndScale(gfx::Vector2d scroll_delta,
                                   float page_scale) OVERRIDE {}
  virtual scoped_ptr<cc::OutputSurface> CreateOutputSurface(bool fallback)
      OVERRIDE;
  virtual void DidInitializeOutputSurface(bool success) OVERRIDE {}
  virtual void WillCommit() OVERRIDE {}
  virtual void DidCommit() OVERRIDE;
  virtual void DidCommitAndDrawFrame() OVERRIDE;
  virtual void DidCompleteSwapBuffers() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProvider() OVERRIDE;

  
  virtual void ScheduleComposite() OVERRIDE;
  virtual void ScheduleAnimation() OVERRIDE;
  virtual void DidPostSwapBuffers() OVERRIDE;
  virtual void DidAbortSwapBuffers() OVERRIDE;

  int last_started_frame() { return last_started_frame_; }
  int last_ended_frame() { return last_ended_frame_; }

  bool IsLocked() { return compositor_lock_ != NULL; }

  const cc::LayerTreeDebugState& GetLayerTreeDebugState() const;
  void SetLayerTreeDebugState(const cc::LayerTreeDebugState& debug_state);

 private:
  friend class base::RefCounted<Compositor>;
  friend class CompositorLock;

  
  void UnlockCompositor();

  
  void CancelCompositorLock();

  
  void NotifyEnd();

  gfx::Size size_;

  
  Layer* root_layer_;

  ObserverList<CompositorObserver> observer_list_;

  gfx::AcceleratedWidget widget_;
  scoped_refptr<cc::Layer> root_web_layer_;
  scoped_ptr<cc::LayerTreeHost> host_;

  
  scoped_ptr<PostedSwapQueue> posted_swaps_;

  
  
  float device_scale_factor_;

  int last_started_frame_;
  int last_ended_frame_;

  bool next_draw_is_resize_;

  bool disable_schedule_composite_;

  CompositorLock* compositor_lock_;

  
  bool defer_draw_scheduling_;

  
  bool waiting_on_compositing_end_;
  bool draw_on_compositing_end_;

  base::WeakPtrFactory<Compositor> schedule_draw_factory_;

  DISALLOW_COPY_AND_ASSIGN(Compositor);
};

}  

#endif  
