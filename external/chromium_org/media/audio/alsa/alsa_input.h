// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_ALSA_ALSA_INPUT_H_
#define MEDIA_AUDIO_ALSA_ALSA_INPUT_H_

#include <alsa/asoundlib.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "media/audio/agc_audio_stream.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AlsaWrapper;
class AudioManagerBase;

class AlsaPcmInputStream : public AgcAudioStream<AudioInputStream> {
 public:
  
  
  static const char kAutoSelectDevice[];

  
  
  
  AlsaPcmInputStream(AudioManagerBase* audio_manager,
                     const std::string& device_name,
                     const AudioParameters& params,
                     AlsaWrapper* wrapper);

  virtual ~AlsaPcmInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;

 private:
  
  void HandleError(const char* method, int error);

  
  
  void ReadAudio();

  
  bool Recover(int error);

  
  snd_pcm_sframes_t GetCurrentDelay();

  
  
  
  
  
  AudioManagerBase* audio_manager_;
  std::string device_name_;
  AudioParameters params_;
  int bytes_per_buffer_;
  AlsaWrapper* wrapper_;
  base::TimeDelta buffer_duration_;  
  AudioInputCallback* callback_;  
  base::TimeTicks next_read_time_;  
  snd_pcm_t* device_handle_;  
  snd_mixer_t* mixer_handle_; 
  snd_mixer_elem_t* mixer_element_handle_; 
  base::WeakPtrFactory<AlsaPcmInputStream> weak_factory_;
  scoped_ptr<uint8[]> audio_buffer_;  
  bool read_callback_behind_schedule_;

  DISALLOW_COPY_AND_ASSIGN(AlsaPcmInputStream);
};

}  

#endif  
