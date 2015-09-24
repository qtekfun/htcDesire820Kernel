// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_PULSE_PULSE_OUTPUT_H_
#define MEDIA_AUDIO_PULSE_PULSE_OUTPUT_H_

#include "base/memory/scoped_ptr.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

struct pa_context;
struct pa_operation;
struct pa_stream;
struct pa_threaded_mainloop;

namespace media {
class AudioManagerBase;

class PulseAudioOutputStream : public AudioOutputStream {
 public:
  PulseAudioOutputStream(const AudioParameters& params,
                         AudioManagerBase* manager);

  virtual ~PulseAudioOutputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

 private:
  
  
  
  static void StreamNotifyCallback(pa_stream* s, void* p_this);

  
  static void StreamRequestCallback(pa_stream* s, size_t len, void* p_this);

  
  
  void FulfillWriteRequest(size_t requested_bytes);

  
  void Reset();

  
  const AudioParameters params_;

  
  AudioManagerBase* manager_;

  
  pa_context* pa_context_;
  pa_threaded_mainloop* pa_mainloop_;
  pa_stream* pa_stream_;

  
  float volume_;

  
  
  AudioSourceCallback* source_callback_;

  
  scoped_ptr<AudioBus> audio_bus_;

  DISALLOW_COPY_AND_ASSIGN(PulseAudioOutputStream);
};

}  

#endif  
