// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_ANDROID_VIDEO_DECODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_ANDROID_VIDEO_DECODE_ACCELERATOR_H_

#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/threading/thread_checker.h"
#include "base/timer/timer.h"
#include "content/common/content_export.h"
#include "content/common/gpu/media/video_decode_accelerator_impl.h"
#include "gpu/command_buffer/service/gles2_cmd_copy_texture_chromium.h"
#include "gpu/command_buffer/service/gles2_cmd_decoder.h"
#include "media/base/android/media_codec_bridge.h"
#include "media/video/video_decode_accelerator.h"

namespace gfx {
class SurfaceTexture;
}

namespace content {
class CONTENT_EXPORT AndroidVideoDecodeAccelerator
    : public VideoDecodeAcceleratorImpl {
 public:
  
  AndroidVideoDecodeAccelerator(
      media::VideoDecodeAccelerator::Client* client,
      const base::WeakPtr<gpu::gles2::GLES2Decoder> decoder,
      const base::Callback<bool(void)>& make_context_current);

  
  virtual bool Initialize(media::VideoCodecProfile profile) OVERRIDE;
  virtual void Decode(const media::BitstreamBuffer& bitstream_buffer) OVERRIDE;
  virtual void AssignPictureBuffers(
      const std::vector<media::PictureBuffer>& buffers) OVERRIDE;
  virtual void ReusePictureBuffer(int32 picture_buffer_id) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void Destroy() OVERRIDE;

 private:
  enum State {
    NO_ERROR,
    ERROR,
  };

  static const base::TimeDelta kDecodePollDelay;

  virtual ~AndroidVideoDecodeAccelerator();

  
  bool ConfigureMediaCodec();

  
  void SendCurrentSurfaceToClient(int32 bitstream_id);

  
  
  
  void DoIOTask();

  
  
  void QueueInput();

  
  
  void DequeueOutput();

  
  void NotifyInitializeDone();

  
  void RequestPictureBuffers();

  
  void NotifyPictureReady(const media::Picture& picture);

  
  
  void NotifyEndOfBitstreamBuffer(int input_buffer_id);

  
  void NotifyFlushDone();

  
  void NotifyResetDone();

  
  void NotifyError(media::VideoDecodeAccelerator::Error error);

  
  base::ThreadChecker thread_checker_;

  
  Client* client_;

  
  base::Callback<bool(void)> make_context_current_;

  
  media::VideoCodec codec_;

  
  
  State state_;

  
  
  typedef std::map<int32, media::PictureBuffer> OutputBufferMap;
  OutputBufferMap output_picture_buffers_;

  
  
  std::queue<int32> free_picture_ids_;

  
  
  
  std::set<int32> dismissed_picture_ids_;

  
  scoped_ptr<media::VideoCodecBridge> media_codec_;

  
  scoped_refptr<gfx::SurfaceTexture> surface_texture_;

  
  uint32 surface_texture_id_;

  
  bool picturebuffers_requested_;

  
  gfx::Size size_;

  
  
  
  typedef std::queue<std::pair<media::BitstreamBuffer, base::Time> >
      PendingBitstreamBuffers;
  PendingBitstreamBuffers pending_bitstream_buffers_;

  
  
  std::list<int32> bitstreams_notified_in_advance_;

  
  base::WeakPtr<gpu::gles2::GLES2Decoder> gl_decoder_;

  
  scoped_ptr<gpu::CopyTextureCHROMIUMResourceManager> copier_;

  
  base::RepeatingTimer<AndroidVideoDecodeAccelerator> io_timer_;

  friend class AndroidVideoDecodeAcceleratorTest;
};

}  

#endif  
