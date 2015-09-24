// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_POWER_MONITOR_H_
#define MEDIA_AUDIO_AUDIO_POWER_MONITOR_H_

#include <limits>
#include <utility>

#include "base/callback.h"
#include "base/synchronization/lock.h"
#include "media/base/media_export.h"


namespace base {
class TimeDelta;
}

namespace media {

class AudioBus;

class MEDIA_EXPORT AudioPowerMonitor {
 public:
  
  
  
  
  AudioPowerMonitor(int sample_rate, const base::TimeDelta& time_constant);

  ~AudioPowerMonitor();

  
  
  void Reset();

  
  
  void Scan(const AudioBus& buffer, int frames);

  
  
  
  
  std::pair<float, bool> ReadCurrentPowerAndClip();

  
  static float zero_power() { return -std::numeric_limits<float>::infinity(); }

  
  static float max_power() { return 0.0f; }

 private:
  
  
  const float sample_weight_;

  
  
  float average_power_;
  bool has_clipped_;

  
  
  base::Lock reading_lock_;
  float power_reading_;
  bool clipped_reading_;

  DISALLOW_COPY_AND_ASSIGN(AudioPowerMonitor);
};

}  

#endif  
