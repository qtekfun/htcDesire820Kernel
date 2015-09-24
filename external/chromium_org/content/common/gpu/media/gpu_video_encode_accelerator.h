// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_GPU_VIDEO_ENCODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_GPU_VIDEO_ENCODE_ACCELERATOR_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ipc/ipc_listener.h"
#include "media/video/video_encode_accelerator.h"
#include "ui/gfx/size.h"

namespace base {

class SharedMemory;

}  

namespace content {

class GpuChannel;

class GpuVideoEncodeAccelerator : public IPC::Listener,
                                  public media::VideoEncodeAccelerator::Client {
 public:
  GpuVideoEncodeAccelerator(GpuChannel* gpu_channel, int32 route_id);
  virtual ~GpuVideoEncodeAccelerator();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual void NotifyInitializeDone() OVERRIDE;
  virtual void RequireBitstreamBuffers(unsigned int input_count,
                                       const gfx::Size& input_coded_size,
                                       size_t output_buffer_size) OVERRIDE;
  virtual void BitstreamBufferReady(int32 bitstream_buffer_id,
                                    size_t payload_size,
                                    bool key_frame) OVERRIDE;
  virtual void NotifyError(media::VideoEncodeAccelerator::Error error) OVERRIDE;

  
  
  static std::vector<media::VideoEncodeAccelerator::SupportedProfile>
      GetSupportedProfiles();

 private:
  
  void CreateEncoder();

  
  
  void OnInitialize(media::VideoFrame::Format input_format,
                    const gfx::Size& input_visible_size,
                    media::VideoCodecProfile output_profile,
                    uint32 initial_bitrate);
  void OnEncode(int32 frame_id,
                base::SharedMemoryHandle buffer_handle,
                uint32 buffer_size,
                bool force_keyframe);
  void OnUseOutputBitstreamBuffer(int32 buffer_id,
                                  base::SharedMemoryHandle buffer_handle,
                                  uint32 buffer_size);
  void OnRequestEncodingParametersChange(uint32 bitrate, uint32 framerate);

  void EncodeFrameFinished(int32 frame_id, scoped_ptr<base::SharedMemory> shm);

  void Send(IPC::Message* message);

  
  base::WeakPtrFactory<GpuVideoEncodeAccelerator> weak_this_factory_;

  
  GpuChannel* channel_;
  const int32 route_id_;

  
  scoped_ptr<media::VideoEncodeAccelerator> encoder_;

  
  media::VideoFrame::Format input_format_;
  gfx::Size input_visible_size_;
  gfx::Size input_coded_size_;
  size_t output_buffer_size_;

  DISALLOW_COPY_AND_ASSIGN(GpuVideoEncodeAccelerator);
};

}  

#endif  
