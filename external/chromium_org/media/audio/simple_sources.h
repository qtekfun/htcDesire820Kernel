// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_SIMPLE_SOURCES_H_
#define MEDIA_AUDIO_SIMPLE_SOURCES_H_

#include "base/synchronization/lock.h"
#include "media/audio/audio_io.h"
#include "media/base/seekable_buffer.h"

namespace media {

class MEDIA_EXPORT SineWaveAudioSource
    : public AudioOutputStream::AudioSourceCallback {
 public:
  
  
  
  SineWaveAudioSource(int channels, double freq, double sample_freq);
  virtual ~SineWaveAudioSource() {}

  
  
  void CapSamples(int cap);
  void Reset();

  
  virtual int OnMoreData(AudioBus* audio_bus,
                         AudioBuffersState audio_buffers) OVERRIDE;
  virtual int OnMoreIOData(AudioBus* source,
                           AudioBus* dest,
                           AudioBuffersState audio_buffers) OVERRIDE;
  virtual void OnError(AudioOutputStream* stream) OVERRIDE;

  
  int callbacks() { return callbacks_; }
  int errors() { return errors_; }

 protected:
  int channels_;
  double f_;
  int time_state_;
  int cap_;
  int callbacks_;
  int errors_;
  base::Lock time_lock_;
};

}  

#endif  
