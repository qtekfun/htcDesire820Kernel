// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef MEDIA_AUDIO_MAC_AUDIO_LOW_LATENCY_OUTPUT_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_LOW_LATENCY_OUTPUT_MAC_H_

#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerMac;

class AUAudioOutputStream : public AudioOutputStream {
 public:
  
  
  AUAudioOutputStream(AudioManagerMac* manager,
                      const AudioParameters& params);
  
  
  virtual ~AUAudioOutputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

  static int HardwareSampleRate();

 private:
  
  static OSStatus InputProc(void* user_data,
                            AudioUnitRenderActionFlags* flags,
                            const AudioTimeStamp* time_stamp,
                            UInt32 bus_number,
                            UInt32 number_of_frames,
                            AudioBufferList* io_data);

  OSStatus Render(UInt32 number_of_frames, AudioBufferList* io_data,
                  const AudioTimeStamp* output_time_stamp);

  
  bool Configure();

  
  
  double GetHardwareLatency();

  
  double GetPlayoutLatency(const AudioTimeStamp* output_time_stamp);

  
  AudioManagerMac* manager_;

  size_t number_of_frames_;

  
  AudioSourceCallback* source_;

  
  
  base::Lock source_lock_;

  
  AudioStreamBasicDescription format_;

  
  AudioUnit output_unit_;

  
  AudioDeviceID output_device_id_;

  
  float volume_;

  
  double hardware_latency_frames_;

  
  bool stopped_;

  
  scoped_ptr<AudioBus> audio_bus_;

  DISALLOW_COPY_AND_ASSIGN(AUAudioOutputStream);
};

}  

#endif  
