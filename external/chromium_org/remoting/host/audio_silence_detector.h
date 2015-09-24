// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_AUDIO_SILENCE_DETECTOR_H_
#define REMOTING_HOST_AUDIO_SILENCE_DETECTOR_H_

#include "base/basictypes.h"
#include "base/logging.h"

namespace remoting {

class AudioSilenceDetector {
 public:
  
  
  AudioSilenceDetector(int threshold);
  ~AudioSilenceDetector();

  void Reset(int sampling_rate, int channels);

  
  
  bool IsSilence(const int16* samples, size_t samples_count);

 private:
  
  int threshold_;

  
  
  
  int silence_length_max_;

  
  int silence_length_;
};

}  

#endif  
