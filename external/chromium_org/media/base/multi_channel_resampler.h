// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_MULTI_CHANNEL_RESAMPLER_H_
#define MEDIA_BASE_MULTI_CHANNEL_RESAMPLER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "media/base/sinc_resampler.h"

namespace media {
class AudioBus;

class MEDIA_EXPORT MultiChannelResampler {
 public:
  
  
  
  
  typedef base::Callback<void(int frame_delay, AudioBus* audio_bus)> ReadCB;

  
  
  
  
  MultiChannelResampler(int channels,
                        double io_sample_rate_ratio,
                        size_t request_frames,
                        const ReadCB& read_cb);
  virtual ~MultiChannelResampler();

  
  void Resample(int frames, AudioBus* audio_bus);

  
  
  void Flush();

  
  
  
  void SetRatio(double io_sample_rate_ratio);

 private:
  
  
  void ProvideInput(int channel, int frames, float* destination);

  
  ReadCB read_cb_;

  
  ScopedVector<SincResampler> resamplers_;

  
  scoped_ptr<AudioBus> resampler_audio_bus_;

  
  
  scoped_ptr<AudioBus> wrapped_resampler_audio_bus_;

  
  
  int output_frames_ready_;

  DISALLOW_COPY_AND_ASSIGN(MultiChannelResampler);
};

}  

#endif  
