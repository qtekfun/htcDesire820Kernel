// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP4_AAC_H_
#define MEDIA_MP4_AAC_H_

#include <vector>

#include "base/basictypes.h"
#include "media/base/channel_layout.h"
#include "media/base/media_export.h"

namespace media {

class BitReader;

namespace mp4 {

class MEDIA_EXPORT AAC {
 public:
  AAC();
  ~AAC();

  
  
  
  
  bool Parse(const std::vector<uint8>& data);

  
  
  
  
  
  int GetOutputSamplesPerSecond(bool sbr_in_mimetype) const;

  
  
  
  
  
  ChannelLayout GetChannelLayout(bool sbr_in_mimetype) const;

  
  
  
  
  bool ConvertEsdsToADTS(std::vector<uint8>* buffer) const;

#if defined(OS_ANDROID)
  
  std::vector<uint8> codec_specific_data() const {
    return codec_specific_data_;
  }
#endif

  
  static const size_t kADTSHeaderSize = 7;

 private:
  bool SkipDecoderGASpecificConfig(BitReader* bit_reader) const;
  bool SkipErrorSpecificConfig() const;
  bool SkipGASpecificConfig(BitReader* bit_reader) const;

  
  
  uint8 profile_;
  uint8 frequency_index_;
  uint8 channel_config_;

#if defined(OS_ANDROID)
  
  std::vector<uint8> codec_specific_data_;
#endif

  
  
  
  
  int frequency_;
  int extension_frequency_;
  ChannelLayout channel_layout_;
};

}  

}  

#endif  
