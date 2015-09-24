// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_VIDEO_FRAME_PROVIDER_CLIENT_IMPL_H_
#define CC_LAYERS_VIDEO_FRAME_PROVIDER_CLIENT_IMPL_H_

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "cc/layers/video_frame_provider.h"
#include "ui/gfx/transform.h"

namespace media { class VideoFrame; }

namespace cc {
class VideoLayerImpl;

class VideoFrameProviderClientImpl
    : public VideoFrameProvider::Client,
      public base::RefCounted<VideoFrameProviderClientImpl> {
 public:
  static scoped_refptr<VideoFrameProviderClientImpl> Create(
      VideoFrameProvider* provider);

  void set_active_video_layer(VideoLayerImpl* video_layer) {
    active_video_layer_ = video_layer;
  }

  void Stop();
  bool Stopped() const { return !provider_; }

  scoped_refptr<media::VideoFrame> AcquireLockAndCurrentFrame();
  void PutCurrentFrame(const scoped_refptr<media::VideoFrame>& frame);
  void ReleaseLock();
  const gfx::Transform& stream_texture_matrix() const {
    return stream_texture_matrix_;
  }

  
  
  virtual void StopUsingProvider() OVERRIDE;
  virtual void DidReceiveFrame() OVERRIDE;
  virtual void DidUpdateMatrix(const float* matrix) OVERRIDE;

 private:
  explicit VideoFrameProviderClientImpl(VideoFrameProvider* provider);
  friend class base::RefCounted<VideoFrameProviderClientImpl>;
  virtual ~VideoFrameProviderClientImpl();

  VideoLayerImpl* active_video_layer_;

  
  base::Lock provider_lock_;
  VideoFrameProvider* provider_;

  gfx::Transform stream_texture_matrix_;

  DISALLOW_COPY_AND_ASSIGN(VideoFrameProviderClientImpl);
};

}  

#endif  
