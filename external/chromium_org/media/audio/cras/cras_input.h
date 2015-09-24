// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_CRAS_CRAS_INPUT_H_
#define MEDIA_AUDIO_CRAS_CRAS_INPUT_H_

#include <cras_client.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "media/audio/agc_audio_stream.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerCras;

class CrasInputStream : public AgcAudioStream<AudioInputStream> {
 public:
  
  
  CrasInputStream(const AudioParameters& params, AudioManagerCras* manager,
                  const std::string& device_id);

  
  
  virtual ~CrasInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;

 private:
  
  
  static int SamplesReady(cras_client* client,
                          cras_stream_id_t stream_id,
                          uint8* samples,
                          size_t frames,
                          const timespec* sample_ts,
                          void* arg);

  
  static int StreamError(cras_client* client,
                         cras_stream_id_t stream_id,
                         int err,
                         void* arg);

  
  
  void ReadAudio(size_t frames, uint8* buffer, const timespec* sample_ts);

  
  void NotifyStreamError(int err);

  
  double GetVolumeRatioFromDecibels(double dB) const;

  
  double GetDecibelsFromVolumeRatio(double volume_ratio) const;

  
  
  
  
  
  AudioManagerCras* const audio_manager_;

  
  uint32 bytes_per_frame_;

  
  AudioInputCallback* callback_;

  
  cras_client* client_;

  
  const AudioParameters params_;

  
  bool started_;

  
  cras_stream_id_t stream_id_;

  
  const CRAS_STREAM_DIRECTION stream_direction_;

  DISALLOW_COPY_AND_ASSIGN(CrasInputStream);
};

}  

#endif  
