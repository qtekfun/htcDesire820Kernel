// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_CLIENT_GPU_VIDEO_ENCODE_ACCELERATOR_HOST_H_
#define CONTENT_COMMON_GPU_CLIENT_GPU_VIDEO_ENCODE_ACCELERATOR_HOST_H_

#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "ipc/ipc_listener.h"
#include "media/video/video_encode_accelerator.h"

namespace gfx {

class Size;

}  

namespace media {

class VideoFrame;

}  

namespace content {

class GpuChannelHost;

class GpuVideoEncodeAcceleratorHost
    : public IPC::Listener,
      public media::VideoEncodeAccelerator,
      public base::SupportsWeakPtr<GpuVideoEncodeAcceleratorHost> {
 public:
  
  
  GpuVideoEncodeAcceleratorHost(
      media::VideoEncodeAccelerator::Client* client,
      const scoped_refptr<GpuChannelHost>& gpu_channel_host,
      int32 route_id);
  virtual ~GpuVideoEncodeAcceleratorHost();

  
  
  static std::vector<SupportedProfile> GetSupportedProfiles();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual void Initialize(media::VideoFrame::Format format,
                          const gfx::Size& input_visible_size,
                          media::VideoCodecProfile output_profile,
                          uint32 initial_bitrate) OVERRIDE;
  virtual void Encode(const scoped_refptr<media::VideoFrame>& frame,
                      bool force_keyframe) OVERRIDE;
  virtual void UseOutputBitstreamBuffer(
      const media::BitstreamBuffer& buffer) OVERRIDE;
  virtual void RequestEncodingParametersChange(uint32 bitrate,
                                               uint32 framerate_num) OVERRIDE;
  virtual void Destroy() OVERRIDE;

 private:
  
  
  void NotifyError(Error error);

  
  
  void OnNotifyInitializeDone();
  void OnRequireBitstreamBuffers(uint32 input_count,
                                 const gfx::Size& input_coded_size,
                                 uint32 output_buffer_size);
  void OnNotifyInputDone(int32 frame_id);
  void OnBitstreamBufferReady(int32 bitstream_buffer_id,
                              uint32 payload_size,
                              bool key_frame);
  void OnNotifyError(Error error);

  void Send(IPC::Message* message);

  
  
  media::VideoEncodeAccelerator::Client* client_;
  
  
  base::WeakPtrFactory<VideoEncodeAccelerator::Client> client_ptr_factory_;

  
  scoped_refptr<GpuChannelHost> channel_;
  const int32 route_id_;

  
  
  typedef base::hash_map<int32, scoped_refptr<media::VideoFrame> > FrameMap;
  FrameMap frame_map_;

  
  int32 next_frame_id_;

  DISALLOW_COPY_AND_ASSIGN(GpuVideoEncodeAcceleratorHost);
};

}  

#endif  
