// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_ANDROID_VIDEO_ENCODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_ANDROID_VIDEO_ENCODE_ACCELERATOR_H_

#include <list>
#include <queue>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "base/timer/timer.h"
#include "base/tuple.h"
#include "content/common/content_export.h"
#include "media/base/android/media_codec_bridge.h"
#include "media/video/video_encode_accelerator.h"

namespace media {
class BitstreamBuffer;
}  

namespace content {

class CONTENT_EXPORT AndroidVideoEncodeAccelerator
    : public media::VideoEncodeAccelerator {
 public:
  explicit AndroidVideoEncodeAccelerator(
      media::VideoEncodeAccelerator::Client* client);
  virtual ~AndroidVideoEncodeAccelerator();

  static std::vector<media::VideoEncodeAccelerator::SupportedProfile>
      GetSupportedProfiles();

  
  virtual void Initialize(media::VideoFrame::Format format,
                          const gfx::Size& input_visible_size,
                          media::VideoCodecProfile output_profile,
                          uint32 initial_bitrate) OVERRIDE;
  virtual void Encode(const scoped_refptr<media::VideoFrame>& frame,
                      bool force_keyframe) OVERRIDE;
  virtual void UseOutputBitstreamBuffer(const media::BitstreamBuffer& buffer)
      OVERRIDE;
  virtual void RequestEncodingParametersChange(uint32 bitrate,
                                               uint32 framerate) OVERRIDE;
  virtual void Destroy() OVERRIDE;

 private:
  enum {
    
    INITIAL_FRAMERATE = 30,
    
    IFRAME_INTERVAL = kint32max,
  };

  
  
  void DoIOTask();
  void QueueInput();
  void DequeueOutput();

  
  bool DoOutputBuffersSuffice();

  
  void MaybeStartIOTimer();
  void MaybeStopIOTimer();

  
  base::ThreadChecker thread_checker_;

  
  
  base::WeakPtrFactory<Client> client_ptr_factory_;

  scoped_ptr<media::VideoCodecBridge> media_codec_;

  
  std::vector<media::BitstreamBuffer> available_bitstream_buffers_;

  
  
  typedef std::queue<
      Tuple3<scoped_refptr<media::VideoFrame>, bool, base::Time> >
      PendingFrames;
  PendingFrames pending_frames_;

  
  base::RepeatingTimer<AndroidVideoEncodeAccelerator> io_timer_;

  
  int32 num_buffers_at_codec_;

  
  
  base::TimeDelta fake_input_timestamp_;

  
  int num_output_buffers_;          
  size_t output_buffers_capacity_;  

  uint32 last_set_bitrate_;  

  DISALLOW_COPY_AND_ASSIGN(AndroidVideoEncodeAccelerator);
};

}  

#endif  
