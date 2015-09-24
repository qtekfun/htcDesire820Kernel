// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_IN_PROCESS_SYNCHRONOUS_COMPOSITOR_FACTORY_IMPL_H_
#define CONTENT_BROWSER_ANDROID_IN_PROCESS_SYNCHRONOUS_COMPOSITOR_FACTORY_IMPL_H_

#include "base/synchronization/lock.h"
#include "content/browser/android/in_process/synchronous_input_event_filter.h"
#include "content/renderer/android/synchronous_compositor_factory.h"
#include "content/renderer/media/android/stream_texture_factory_android_synchronous_impl.h"
#include "gpu/command_buffer/service/in_process_command_buffer.h"

namespace gfx {
class GLSurface;
}

namespace gpu {
class GLInProcessContext;
}

namespace webkit {
namespace gpu {
class WebGraphicsContext3DInProcessCommandBufferImpl;
}
}

namespace content {

class SynchronousCompositorFactoryImpl : public SynchronousCompositorFactory {
 public:
  SynchronousCompositorFactoryImpl();
  virtual ~SynchronousCompositorFactoryImpl();

  
  virtual scoped_refptr<base::MessageLoopProxy> GetCompositorMessageLoop()
      OVERRIDE;
  virtual scoped_ptr<cc::OutputSurface> CreateOutputSurface(int routing_id)
      OVERRIDE;
  virtual InputHandlerManagerClient* GetInputHandlerManagerClient() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      GetOffscreenContextProviderForMainThread() OVERRIDE;
  
  
  
  
  
  
  virtual scoped_refptr<cc::ContextProvider>
      GetOffscreenContextProviderForCompositorThread() OVERRIDE;
  virtual scoped_ptr<StreamTextureFactory> CreateStreamTextureFactory(
      int view_id) OVERRIDE;

  SynchronousInputEventFilter* synchronous_input_event_filter() {
    return &synchronous_input_event_filter_;
  }

  void SetDeferredGpuService(
      scoped_refptr<gpu::InProcessCommandBuffer::Service> service);
  void CompositorInitializedHardwareDraw();
  void CompositorReleasedHardwareDraw();

  scoped_refptr<cc::ContextProvider>
      CreateOnscreenContextProviderForCompositorThread(
          scoped_refptr<gfx::GLSurface> surface);

 private:
  bool CanCreateMainThreadContext();
  scoped_refptr<StreamTextureFactorySynchronousImpl::ContextProvider>
      TryCreateStreamTextureFactory();

  SynchronousInputEventFilter synchronous_input_event_filter_;

  
  
  base::Lock offscreen_context_for_compositor_thread_lock_;
  scoped_refptr<cc::ContextProvider> offscreen_context_for_main_thread_;
  
  
  gpu::GLInProcessContext* wrapped_gl_context_for_main_thread_;
  gpu::GLInProcessContext* wrapped_gl_context_for_compositor_thread_;
  scoped_refptr<cc::ContextProvider> offscreen_context_for_compositor_thread_;

  scoped_refptr<gpu::InProcessCommandBuffer::Service> service_;
  scoped_refptr<StreamTextureFactorySynchronousImpl::ContextProvider>
      video_context_provider_;

  
  
  base::Lock num_hardware_compositor_lock_;
  unsigned int num_hardware_compositors_;
};

}  

#endif  
