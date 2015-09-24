// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef MEDIA_AUDIO_MAC_AUDIO_AUHAL_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_AUHAL_MAC_H_

#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerMac;
class AudioPullFifo;


class AUHALStream : public AudioOutputStream {
 public:
  
  
  
  AUHALStream(AudioManagerMac* manager,
              const AudioParameters& params,
              AudioDeviceID device);
  
  
  virtual ~AUHALStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

 private:
  
  static OSStatus InputProc(void* user_data,
                            AudioUnitRenderActionFlags* flags,
                            const AudioTimeStamp* time_stamp,
                            UInt32 bus_number,
                            UInt32 number_of_frames,
                            AudioBufferList* io_data);

  OSStatus Render(AudioUnitRenderActionFlags* flags,
                  const AudioTimeStamp* output_time_stamp,
                  UInt32 bus_number,
                  UInt32 number_of_frames,
                  AudioBufferList* io_data);

  
  void ProvideInput(int frame_delay, AudioBus* dest);

  
  bool EnableIO(bool enable, UInt32 scope);

  
  
  
  bool SetStreamFormat(AudioStreamBasicDescription* desc,
                       int channels,
                       UInt32 scope,
                       UInt32 element);

  
  bool ConfigureAUHAL();

  
  void CreateIOBusses();

  
  
  double GetHardwareLatency();

  
  double GetPlayoutLatency(const AudioTimeStamp* output_time_stamp);

  
  AudioManagerMac* const manager_;

  const AudioParameters params_;
  
  const int input_channels_;
  const int output_channels_;

  
  const size_t number_of_frames_;

  
  AudioSourceCallback* source_;

  
  
  base::Lock source_lock_;

  
  AudioStreamBasicDescription input_format_;
  AudioStreamBasicDescription output_format_;

  
  
  const AudioDeviceID device_;

  
  AudioUnit audio_unit_;

  
  float volume_;

  
  double hardware_latency_frames_;

  
  bool stopped_;

  
  scoped_ptr<uint8[]> input_buffer_list_storage_;
  AudioBufferList* input_buffer_list_;

  
  scoped_ptr<AudioBus> input_bus_;

  
  scoped_ptr<AudioBus> output_bus_;

  
  
  scoped_ptr<AudioPullFifo> audio_fifo_;

  
  uint32 current_hardware_pending_bytes_;

  DISALLOW_COPY_AND_ASSIGN(AUHALStream);
};

}  

#endif  
