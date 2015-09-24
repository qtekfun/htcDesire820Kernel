// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_FFMPEG_H264_TO_ANNEX_B_BITSTREAM_CONVERTER_H_
#define MEDIA_FILTERS_FFMPEG_H264_TO_ANNEX_B_BITSTREAM_CONVERTER_H_

#include "base/basictypes.h"
#include "media/filters/h264_to_annex_b_bitstream_converter.h"

struct AVCodecContext;
struct AVPacket;

namespace media {

class MEDIA_EXPORT FFmpegH264ToAnnexBBitstreamConverter {
 public:
  
  
  
  explicit FFmpegH264ToAnnexBBitstreamConverter(AVCodecContext* stream_context);
  ~FFmpegH264ToAnnexBBitstreamConverter();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool ConvertPacket(AVPacket* packet);

 private:
  
  H264ToAnnexBBitstreamConverter converter_;

  
  bool configuration_processed_;

  
  
  AVCodecContext* stream_context_;

  DISALLOW_COPY_AND_ASSIGN(FFmpegH264ToAnnexBBitstreamConverter);
};

}  

#endif  
