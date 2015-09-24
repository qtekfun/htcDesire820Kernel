// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_PULSE_PULSE_UNIFIED_H_
#define MEDIA_AUDIO_PULSE_PULSE_UNIFIED_H_

#include <pulse/pulseaudio.h>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_fifo.h"

namespace media {

class AudioManagerBase;
class SeekableBuffer;

class PulseAudioUnifiedStream : public AudioOutputStream {
 public:
  PulseAudioUnifiedStream(const AudioParameters& params,
                          const std::string& input_device_id,
                          AudioManagerBase* manager);

  virtual ~PulseAudioUnifiedStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

 private:
  
  
  
  static void StreamNotifyCallback(pa_stream* s, void* user_data);

  
  static void ReadCallback(pa_stream* s, size_t length, void* user_data);

  
  void WriteData(size_t requested_bytes);
  void ReadData();

  
  void Reset();

  
  const AudioParameters params_;

  
  const std::string input_device_id_;

  
  AudioManagerBase* manager_;

  
  pa_context* pa_context_;
  pa_threaded_mainloop* pa_mainloop_;
  pa_stream* input_stream_;
  pa_stream* output_stream_;

  
  float volume_;

  
  
  AudioSourceCallback* source_callback_;

  scoped_ptr<AudioBus> input_bus_;
  scoped_ptr<AudioBus> output_bus_;

  
  scoped_ptr<media::SeekableBuffer> fifo_;

  
  
  scoped_ptr<uint8[]> input_data_buffer_;

  DISALLOW_COPY_AND_ASSIGN(PulseAudioUnifiedStream);
};

}  

#endif  
