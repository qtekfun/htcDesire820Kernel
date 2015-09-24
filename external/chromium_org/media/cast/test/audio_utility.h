// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_TEST_AUDIO_UTILITY_H_
#define MEDIA_CAST_TEST_AUDIO_UTILITY_H_

#include "media/audio/simple_sources.h"

namespace base {
class TimeDelta;
}

namespace media {
class AudioBus;
}

namespace media {
namespace cast {

struct PcmAudioFrame;

class TestAudioBusFactory {
 public:
  TestAudioBusFactory(int num_channels, int sample_rate,
                      float sine_wave_frequency, float volume);
  ~TestAudioBusFactory();

  
  
  scoped_ptr<AudioBus> NextAudioBus(const base::TimeDelta& duration);

  
  static const int kMiddleANoteFreq = 440;

 private:
  const int num_channels_;
  const int sample_rate_;
  const float volume_;
  SineWaveAudioSource source_;

  DISALLOW_COPY_AND_ASSIGN(TestAudioBusFactory);
};

scoped_ptr<PcmAudioFrame> ToPcmAudioFrame(const AudioBus& audio_bus,
                                          int sample_rate);

int CountZeroCrossings(const std::vector<int16>& samples);

}  
}  

#endif  
