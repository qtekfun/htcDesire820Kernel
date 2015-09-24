// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_LINUX_CRAS_UNIFIED_H_
#define MEDIA_AUDIO_LINUX_CRAS_UNIFIED_H_

#include <alsa/asoundlib.h>
#include <cras_client.h>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerCras;
class AudioParameters;

class MEDIA_EXPORT CrasUnifiedStream : public AudioOutputStream {
 public:
  
  
  CrasUnifiedStream(const AudioParameters& params, AudioManagerCras* manager);

  
  
  virtual ~CrasUnifiedStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

 private:
  
  uint32 GetBytesLatency(const struct timespec& latency);

  
  static int UnifiedCallback(cras_client* client,
                             cras_stream_id_t stream_id,
                             uint8* input_samples,
                             uint8* output_samples,
                             unsigned int frames,
                             const timespec* input_ts,
                             const timespec* output_ts,
                             void* arg);

  
  static int StreamError(cras_client* client,
                         cras_stream_id_t stream_id,
                         int err,
                         void* arg);

  
  uint32 DispatchCallback(size_t frames,
                          uint8* input_samples,
                          uint8* output_samples,
                          const timespec* input_ts,
                          const timespec* output_ts);

  
  uint32 ReadWriteAudio(size_t frames,
                        uint8* input_samples,
                        uint8* output_samples,
                        const timespec* input_ts,
                        const timespec* output_ts);

  
  uint32 WriteAudio(size_t frames, uint8* buffer, const timespec* sample_ts);

  
  void NotifyStreamError(int err);

  
  cras_client* client_;

  
  cras_stream_id_t stream_id_;

  
  AudioParameters params_;

  
  uint32 bytes_per_frame_;

  
  bool is_playing_;

  
  float volume_;

  
  AudioManagerCras* manager_;

  
  AudioSourceCallback* source_callback_;

  
  scoped_ptr<AudioBus> input_bus_;
  scoped_ptr<AudioBus> output_bus_;

  
  CRAS_STREAM_DIRECTION stream_direction_;

  DISALLOW_COPY_AND_ASSIGN(CrasUnifiedStream);
};

}  

#endif  
