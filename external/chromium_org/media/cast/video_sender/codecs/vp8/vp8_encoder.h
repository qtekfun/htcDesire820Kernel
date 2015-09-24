// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_VIDEO_SENDER_CODECS_VP8_VP8_ENCODER_H_
#define MEDIA_CAST_VIDEO_SENDER_CODECS_VP8_VP8_ENCODER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/cast/cast_config.h"
#include "third_party/libvpx/source/libvpx/vpx/vpx_encoder.h"

namespace media {
class VideoFrame;
}

typedef struct vpx_codec_ctx vpx_enc_ctx_t;

namespace media {
namespace cast {

const int kNumberOfVp8VideoBuffers = 3;

class Vp8Encoder {
 public:
  Vp8Encoder(const VideoSenderConfig& video_config,
             uint8 max_unacked_frames);

  ~Vp8Encoder();

  
  bool Encode(const scoped_refptr<media::VideoFrame>& video_frame,
              EncodedVideoFrame* encoded_image);

  
  void UpdateRates(uint32 new_bitrate);

  
  void GenerateKeyFrame();

  void LatestFrameIdToReference(uint32 frame_id);

 private:
  enum Vp8Buffers {
    kAltRefBuffer = 0,
    kGoldenBuffer = 1,
    kLastBuffer = 2,
    kNoBuffer = 3  
  };

  void InitEncode(int number_of_cores);

  
  uint32 MaxIntraTarget(uint32 optimal_buffer_size) const;

  
  Vp8Buffers GetNextBufferToUpdate();

  
  uint32 GetLatestFrameIdToReference();

  
  void GetCodecReferenceFlags(vpx_codec_flags_t* flags);

  
  void GetCodecUpdateFlags(Vp8Buffers buffer_to_update,
                           vpx_codec_flags_t* flags);

  const VideoSenderConfig cast_config_;
  const bool use_multiple_video_buffers_;
  const int max_number_of_repeated_buffers_in_a_row_;

  
  scoped_ptr<vpx_codec_enc_cfg_t> config_;
  vpx_enc_ctx_t* encoder_;
  vpx_image_t* raw_image_;

  bool key_frame_requested_;
  int64 timestamp_;
  uint32 last_encoded_frame_id_;
  uint32 used_buffers_frame_id_[kNumberOfVp8VideoBuffers];
  bool acked_frame_buffers_[kNumberOfVp8VideoBuffers];
  Vp8Buffers last_used_vp8_buffer_;
  int number_of_repeated_buffers_;
};

}  
}  

#endif  
