// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_MAC_AUDIO_UNIFIED_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_UNIFIED_MAC_H_

#include <CoreAudio/CoreAudio.h>

#include "base/memory/scoped_ptr.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerMac;

class AudioHardwareUnifiedStream : public AudioOutputStream {
 public:
  
  
  AudioHardwareUnifiedStream(AudioManagerMac* manager,
                             const AudioParameters& params);
  
  
  virtual ~AudioHardwareUnifiedStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

  int input_channels() const { return input_channels_; }
  int output_channels() const { return output_channels_; }

 private:
  OSStatus Render(AudioDeviceID device,
                  const AudioTimeStamp* now,
                  const AudioBufferList* input_data,
                  const AudioTimeStamp* input_time,
                  AudioBufferList* output_data,
                  const AudioTimeStamp* output_time);

  static OSStatus RenderProc(AudioDeviceID device,
                             const AudioTimeStamp* now,
                             const AudioBufferList* input_data,
                             const AudioTimeStamp* input_time,
                             AudioBufferList* output_data,
                             const AudioTimeStamp* output_time,
                             void* user_data);

  
  AudioManagerMac* manager_;

  
  AudioSourceCallback* source_;

  
  AudioStreamBasicDescription format_;

  
  int number_of_frames_;

  
  int client_input_channels_;

  
  float volume_;

  
  int input_channels_;
  int output_channels_;
  int input_channels_per_frame_;
  int output_channels_per_frame_;

  AudioDeviceIOProcID io_proc_id_;
  AudioDeviceID device_;
  bool is_playing_;

  
  scoped_ptr<AudioBus> input_bus_;
  scoped_ptr<AudioBus> output_bus_;

  DISALLOW_COPY_AND_ASSIGN(AudioHardwareUnifiedStream);
};

}  

#endif  
