// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_DECODER_CONFIG_H_
#define MEDIA_BASE_AUDIO_DECODER_CONFIG_H_

#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "media/base/channel_layout.h"
#include "media/base/media_export.h"
#include "media/base/sample_format.h"

namespace media {

enum AudioCodec {
  
  
  
  kUnknownAudioCodec = 0,
  kCodecAAC,
  kCodecMP3,
  kCodecPCM,
  kCodecVorbis,
  kCodecFLAC,
  kCodecAMR_NB,
  kCodecAMR_WB,
  kCodecPCM_MULAW,
  kCodecGSM_MS,
  kCodecPCM_S16BE,
  kCodecPCM_S24BE,
  kCodecOpus,
  kCodecEAC3,
  kCodecPCM_ALAW,
  
  
  
  

  
  kAudioCodecMax
};

class MEDIA_EXPORT AudioDecoderConfig {
 public:
  
  
  AudioDecoderConfig();

  
  
  AudioDecoderConfig(AudioCodec codec, SampleFormat sample_format,
                     ChannelLayout channel_layout, int samples_per_second,
                     const uint8* extra_data, size_t extra_data_size,
                     bool is_encrypted);

  ~AudioDecoderConfig();

  
  void Initialize(AudioCodec codec, SampleFormat sample_format,
                  ChannelLayout channel_layout, int samples_per_second,
                  const uint8* extra_data, size_t extra_data_size,
                  bool is_encrypted, bool record_stats,
                  base::TimeDelta seek_preroll,
                  base::TimeDelta codec_delay);

  
  
  bool IsValidConfig() const;

  
  
  bool Matches(const AudioDecoderConfig& config) const;

  AudioCodec codec() const { return codec_; }
  int bits_per_channel() const { return bytes_per_channel_ * 8; }
  int bytes_per_channel() const { return bytes_per_channel_; }
  ChannelLayout channel_layout() const { return channel_layout_; }
  int samples_per_second() const { return samples_per_second_; }
  SampleFormat sample_format() const { return sample_format_; }
  int bytes_per_frame() const { return bytes_per_frame_; }
  base::TimeDelta seek_preroll() const { return seek_preroll_; }
  base::TimeDelta codec_delay() const { return codec_delay_; }

  
  
  const uint8* extra_data() const {
    return extra_data_.empty() ? NULL : &extra_data_[0];
  }
  size_t extra_data_size() const { return extra_data_.size(); }

  
  
  
  bool is_encrypted() const { return is_encrypted_; }

 private:
  AudioCodec codec_;
  SampleFormat sample_format_;
  int bytes_per_channel_;
  ChannelLayout channel_layout_;
  int samples_per_second_;
  int bytes_per_frame_;
  std::vector<uint8> extra_data_;
  bool is_encrypted_;

  
  
  base::TimeDelta seek_preroll_;

  
  
  
  base::TimeDelta codec_delay_;

  
  
  
};

}  

#endif  
