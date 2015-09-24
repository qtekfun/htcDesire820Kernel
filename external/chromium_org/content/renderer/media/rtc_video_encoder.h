// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_RTC_VIDEO_ENCODER_H_
#define CONTENT_RENDERER_MEDIA_RTC_VIDEO_ENCODER_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "media/base/video_decoder_config.h"
#include "third_party/webrtc/modules/video_coding/codecs/interface/video_codec_interface.h"
#include "ui/gfx/size.h"

namespace base {

class MessageLoopProxy;

}  

namespace content {

class RendererGpuVideoAcceleratorFactories;

class CONTENT_EXPORT RTCVideoEncoder
    : NON_EXPORTED_BASE(public webrtc::VideoEncoder) {
 public:
  RTCVideoEncoder(
      webrtc::VideoCodecType type,
      media::VideoCodecProfile profile,
      const scoped_refptr<RendererGpuVideoAcceleratorFactories>& gpu_factories);
  virtual ~RTCVideoEncoder();

  
  
  virtual int32_t InitEncode(const webrtc::VideoCodec* codec_settings,
                             int32_t number_of_cores,
                             uint32_t max_payload_size) OVERRIDE;
  virtual int32_t Encode(
      const webrtc::I420VideoFrame& input_image,
      const webrtc::CodecSpecificInfo* codec_specific_info,
      const std::vector<webrtc::VideoFrameType>* frame_types) OVERRIDE;
  virtual int32_t RegisterEncodeCompleteCallback(
      webrtc::EncodedImageCallback* callback) OVERRIDE;
  virtual int32_t Release() OVERRIDE;
  virtual int32_t SetChannelParameters(uint32_t packet_loss, int rtt) OVERRIDE;
  virtual int32_t SetRates(uint32_t new_bit_rate, uint32_t frame_rate) OVERRIDE;

 private:
  class Impl;
  friend class RTCVideoEncoder::Impl;

  
  void ReturnEncodedImage(scoped_ptr<webrtc::EncodedImage> image,
                          int32 bitstream_buffer_id);

  void NotifyError(int32_t error);

  void RecordInitEncodeUMA(int32_t init_retval);

  base::ThreadChecker thread_checker_;

  
  const webrtc::VideoCodecType video_codec_type_;

  
  const media::VideoCodecProfile video_codec_profile_;

  
  scoped_refptr<RendererGpuVideoAcceleratorFactories> gpu_factories_;

  
  webrtc::EncodedImageCallback* encoded_image_callback_;

  
  scoped_refptr<Impl> impl_;

  
  
  
  
  int32_t impl_status_;

  
  
  base::WeakPtrFactory<RTCVideoEncoder> weak_this_factory_;

  DISALLOW_COPY_AND_ASSIGN(RTCVideoEncoder);
};

}  

#endif  
