// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_CLIENT_GPU_VIDEO_DECODE_ACCELERATOR_HOST_H_
#define CONTENT_COMMON_GPU_CLIENT_GPU_VIDEO_DECODE_ACCELERATOR_HOST_H_

#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/gpu/client/command_buffer_proxy_impl.h"
#include "ipc/ipc_listener.h"
#include "media/video/video_decode_accelerator.h"
#include "ui/gfx/size.h"

namespace content {
class GpuChannelHost;

class GpuVideoDecodeAcceleratorHost
    : public IPC::Listener,
      public media::VideoDecodeAccelerator,
      public CommandBufferProxyImpl::DeletionObserver,
      public base::NonThreadSafe {
 public:
  
  GpuVideoDecodeAcceleratorHost(GpuChannelHost* channel,
                                int32 decoder_route_id,
                                media::VideoDecodeAccelerator::Client* client,
                                CommandBufferProxyImpl* impl);

  
  virtual void OnChannelError() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Initialize(media::VideoCodecProfile profile) OVERRIDE;
  virtual void Decode(const media::BitstreamBuffer& bitstream_buffer) OVERRIDE;
  virtual void AssignPictureBuffers(
      const std::vector<media::PictureBuffer>& buffers) OVERRIDE;
  virtual void ReusePictureBuffer(int32 picture_buffer_id) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void Destroy() OVERRIDE;

  
  virtual void OnWillDeleteImpl() OVERRIDE;

 private:
  
  virtual ~GpuVideoDecodeAcceleratorHost();

  void Send(IPC::Message* message);

  void OnBitstreamBufferProcessed(int32 bitstream_buffer_id);
  void OnProvidePictureBuffer(uint32 num_requested_buffers,
                              const gfx::Size& dimensions,
                              uint32 texture_target);
  void OnDismissPictureBuffer(int32 picture_buffer_id);
  void OnPictureReady(int32 picture_buffer_id, int32 bitstream_buffer_id);
  void OnFlushDone();
  void OnResetDone();
  void OnErrorNotification(uint32 error);

  
  GpuChannelHost* channel_;

  
  int32 decoder_route_id_;

  
  media::VideoDecodeAccelerator::Client* client_;

  
  CommandBufferProxyImpl* impl_;

  
  gfx::Size picture_buffer_dimensions_;

  DISALLOW_COPY_AND_ASSIGN(GpuVideoDecodeAcceleratorHost);
};

}  

#endif  
