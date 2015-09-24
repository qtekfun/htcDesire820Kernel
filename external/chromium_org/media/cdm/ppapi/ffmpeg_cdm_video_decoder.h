// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CDM_PPAPI_FFMPEG_CDM_VIDEO_DECODER_H_
#define MEDIA_CDM_PPAPI_FFMPEG_CDM_VIDEO_DECODER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "media/cdm/ppapi/cdm_video_decoder.h"
#include "media/cdm/ppapi/clear_key_cdm_common.h"

struct AVCodecContext;
struct AVFrame;

namespace media {

class ScopedPtrAVFreeContext;
class ScopedPtrAVFreeFrame;

class FFmpegCdmVideoDecoder : public CdmVideoDecoder {
 public:
  explicit FFmpegCdmVideoDecoder(ClearKeyCdmHost* host);
  virtual ~FFmpegCdmVideoDecoder();

  
  virtual bool Initialize(const cdm::VideoDecoderConfig& config) OVERRIDE;
  virtual void Deinitialize() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual cdm::Status DecodeFrame(const uint8_t* compressed_frame,
                                  int32_t compressed_frame_size,
                                  int64_t timestamp,
                                  cdm::VideoFrame* decoded_frame) OVERRIDE;
  virtual bool is_initialized() const OVERRIDE { return is_initialized_; }

  
  
  static bool IsValidOutputConfig(cdm::VideoFormat format,
                                  const cdm::Size& data_size);

 private:
  
  
  bool CopyAvFrameTo(cdm::VideoFrame* cdm_video_frame);

  void ReleaseFFmpegResources();

  
  scoped_ptr_malloc<AVCodecContext, ScopedPtrAVFreeContext> codec_context_;
  scoped_ptr_malloc<AVFrame, ScopedPtrAVFreeFrame> av_frame_;

  bool is_initialized_;

  ClearKeyCdmHost* const host_;

  DISALLOW_COPY_AND_ASSIGN(FFmpegCdmVideoDecoder);
};

}  

#endif  
