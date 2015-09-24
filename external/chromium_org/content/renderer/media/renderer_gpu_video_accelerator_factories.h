// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_RENDERER_GPU_VIDEO_ACCELERATOR_FACTORIES_H_
#define CONTENT_RENDERER_MEDIA_RENDERER_GPU_VIDEO_ACCELERATOR_FACTORIES_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "content/child/thread_safe_sender.h"
#include "content/common/content_export.h"
#include "media/filters/gpu_video_accelerator_factories.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/size.h"

namespace base {
class MessageLoopProxy;
class WaitableEvent;
}

namespace content {
class ContextProviderCommandBuffer;
class GpuChannelHost;
class WebGraphicsContext3DCommandBufferImpl;

class CONTENT_EXPORT RendererGpuVideoAcceleratorFactories
    : public media::GpuVideoAcceleratorFactories {
 public:
  
  
  RendererGpuVideoAcceleratorFactories(
      GpuChannelHost* gpu_channel_host,
      const scoped_refptr<ContextProviderCommandBuffer>& context_provider);

  
  
  virtual scoped_ptr<media::VideoDecodeAccelerator>
      CreateVideoDecodeAccelerator(
          media::VideoCodecProfile profile,
          media::VideoDecodeAccelerator::Client* client) OVERRIDE;
  virtual scoped_ptr<media::VideoEncodeAccelerator>
      CreateVideoEncodeAccelerator(
          media::VideoEncodeAccelerator::Client* client) OVERRIDE;
  
  
  virtual uint32 CreateTextures(int32 count,
                                const gfx::Size& size,
                                std::vector<uint32>* texture_ids,
                                std::vector<gpu::Mailbox>* texture_mailboxes,
                                uint32 texture_target) OVERRIDE;
  virtual void DeleteTexture(uint32 texture_id) OVERRIDE;
  virtual void WaitSyncPoint(uint32 sync_point) OVERRIDE;
  
  virtual void ReadPixels(uint32 texture_id,
                          const gfx::Size& size,
                          const SkBitmap& pixels) OVERRIDE;
  virtual base::SharedMemory* CreateSharedMemory(size_t size) OVERRIDE;
  virtual scoped_refptr<base::MessageLoopProxy> GetMessageLoop() OVERRIDE;
  virtual void Abort() OVERRIDE;
  virtual bool IsAborted() OVERRIDE;
  scoped_refptr<RendererGpuVideoAcceleratorFactories> Clone();

 protected:
  friend class base::RefCountedThreadSafe<RendererGpuVideoAcceleratorFactories>;
  virtual ~RendererGpuVideoAcceleratorFactories();

 private:
  RendererGpuVideoAcceleratorFactories();

  
  
  WebGraphicsContext3DCommandBufferImpl* GetContext3d();

  
  
  void AsyncBindContext();

  
  
  
  
  
  void AsyncCreateVideoDecodeAccelerator(
      media::VideoCodecProfile profile,
      media::VideoDecodeAccelerator::Client* client);
  void AsyncReadPixels(uint32 texture_id, const gfx::Size& size);
  void AsyncDestroyVideoDecodeAccelerator();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  scoped_refptr<GpuChannelHost> gpu_channel_host_;
  scoped_refptr<ContextProviderCommandBuffer> context_provider_;

  
  scoped_refptr<ThreadSafeSender> thread_safe_sender_;

  
  base::WaitableEvent aborted_waiter_;

  
  
  
  base::WaitableEvent message_loop_async_waiter_;

  
  scoped_ptr<media::VideoDecodeAccelerator> vda_;

  
  SkBitmap read_pixels_bitmap_;

  DISALLOW_COPY_AND_ASSIGN(RendererGpuVideoAcceleratorFactories);
};

}  

#endif  
