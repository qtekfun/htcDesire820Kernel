// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_GPU_VIDEO_DECODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_GPU_VIDEO_DECODE_ACCELERATOR_H_

#include <map>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/shared_memory.h"
#include "base/synchronization/waitable_event.h"
#include "content/common/gpu/gpu_command_buffer_stub.h"
#include "content/common/gpu/media/video_decode_accelerator_impl.h"
#include "gpu/command_buffer/service/texture_manager.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "media/video/video_decode_accelerator.h"
#include "ui/gfx/size.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class GpuVideoDecodeAccelerator
    : public IPC::Listener,
      public IPC::Sender,
      public media::VideoDecodeAccelerator::Client,
      public GpuCommandBufferStub::DestructionObserver {
 public:
  
  
  
  GpuVideoDecodeAccelerator(
      int32 host_route_id,
      GpuCommandBufferStub* stub,
      const scoped_refptr<base::MessageLoopProxy>& io_message_loop);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void ProvidePictureBuffers(uint32 requested_num_of_buffers,
                                     const gfx::Size& dimensions,
                                     uint32 texture_target) OVERRIDE;
  virtual void DismissPictureBuffer(int32 picture_buffer_id) OVERRIDE;
  virtual void PictureReady(const media::Picture& picture) OVERRIDE;
  virtual void NotifyInitializeDone() OVERRIDE;
  virtual void NotifyError(media::VideoDecodeAccelerator::Error error) OVERRIDE;
  virtual void NotifyEndOfBitstreamBuffer(int32 bitstream_buffer_id) OVERRIDE;
  virtual void NotifyFlushDone() OVERRIDE;
  virtual void NotifyResetDone() OVERRIDE;

  
  virtual void OnWillDestroyStub() OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  
  
  void Initialize(const media::VideoCodecProfile profile,
                  IPC::Message* init_done_msg);

 private:
  class MessageFilter;

  
  virtual ~GpuVideoDecodeAccelerator();

  
  void OnDecode(base::SharedMemoryHandle handle, int32 id, uint32 size);
  void OnAssignPictureBuffers(const std::vector<int32>& buffer_ids,
                              const std::vector<uint32>& texture_ids);
  void OnReusePictureBuffer(int32 picture_buffer_id);
  void OnFlush();
  void OnReset();
  void OnDestroy();

  
  void OnFilterRemoved();

  
  void SetTextureCleared(const media::Picture& picture);

  
  
  
  IPC::Message* init_done_msg_;

  
  int32 host_route_id_;

  
  GpuCommandBufferStub* stub_;

  
  scoped_ptr<VideoDecodeAcceleratorImpl> video_decode_accelerator_;

  
  
  base::Callback<bool(void)> make_context_current_;

  
  gfx::Size texture_dimensions_;

  
  uint32 texture_target_;

  
  scoped_refptr<MessageFilter> filter_;

  
  
  base::WaitableEvent filter_removed_;

  
  scoped_refptr<base::MessageLoopProxy> child_message_loop_;

  
  scoped_refptr<base::MessageLoopProxy> io_message_loop_;

  
  base::WeakPtrFactory<Client> weak_factory_for_io_;

  
  
  
  base::Lock debug_uncleared_textures_lock_;

  
  std::map<int32, scoped_refptr<gpu::gles2::TextureRef> > uncleared_textures_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(GpuVideoDecodeAccelerator);
};

}  

#endif  
