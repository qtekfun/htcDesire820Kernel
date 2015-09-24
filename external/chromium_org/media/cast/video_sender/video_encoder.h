// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_VIDEO_SENDER_VIDEO_ENCODER_H_
#define MEDIA_CAST_VIDEO_SENDER_VIDEO_ENCODER_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"
#include "media/cast/video_sender/codecs/vp8/vp8_encoder.h"

namespace media {
class VideoFrame;
}

namespace media {
namespace cast {

class VideoEncoder : public VideoEncoderController {
 public:
  typedef base::Callback<void(scoped_ptr<EncodedVideoFrame>,
                              const base::TimeTicks&)> FrameEncodedCallback;

  VideoEncoder(scoped_refptr<CastEnvironment> cast_environment,
               const VideoSenderConfig& video_config,
               uint8 max_unacked_frames);

  virtual ~VideoEncoder();

  
  
  
  
  
  
  
  bool EncodeVideoFrame(const scoped_refptr<media::VideoFrame>& video_frame,
                        const base::TimeTicks& capture_time,
                        const FrameEncodedCallback& frame_encoded_callback);

 protected:
  struct CodecDynamicConfig {
    bool key_frame_requested;
    uint32 latest_frame_id_to_reference;
    int bit_rate;
  };

  
  void EncodeVideoFrameEncoderThread(
      const scoped_refptr<media::VideoFrame>& video_frame,
      const base::TimeTicks& capture_time,
      const CodecDynamicConfig& dynamic_config,
      const FrameEncodedCallback& frame_encoded_callback);

  
  virtual void SetBitRate(int new_bit_rate) OVERRIDE;
  virtual void SkipNextFrame(bool skip_next_frame) OVERRIDE;
  virtual void GenerateKeyFrame() OVERRIDE;
  virtual void LatestFrameIdToReference(uint32 frame_id) OVERRIDE;
  virtual int NumberOfSkippedFrames() const OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<VideoEncoder>;

  const VideoSenderConfig video_config_;
  scoped_refptr<CastEnvironment> cast_environment_;
  scoped_ptr<Vp8Encoder> vp8_encoder_;
  CodecDynamicConfig dynamic_config_;
  bool skip_next_frame_;
  int skip_count_;

  DISALLOW_COPY_AND_ASSIGN(VideoEncoder);
};

}  
}  

#endif  
