// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_MAC_AUDIO_SYNCHRONIZED_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_SYNCHRONIZED_MAC_H_

#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_bus.h"
#include "media/base/audio_fifo.h"

namespace media {

class AudioManagerMac;
class ChannelMixer;

class AudioSynchronizedStream : public AudioOutputStream {
 public:
  
  
  AudioSynchronizedStream(AudioManagerMac* manager,
                          const AudioParameters& params,
                          AudioDeviceID input_id,
                          AudioDeviceID output_id);

  virtual ~AudioSynchronizedStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;

  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

  OSStatus SetInputDeviceAsCurrent(AudioDeviceID input_id);
  OSStatus SetOutputDeviceAsCurrent(AudioDeviceID output_id);
  AudioDeviceID GetInputDeviceID() { return input_info_.id_;  }
  AudioDeviceID GetOutputDeviceID() { return output_info_.id_; }

  bool IsRunning();

 private:
  
  OSStatus CreateAudioUnits();
  OSStatus SetupInput(AudioDeviceID input_id);
  OSStatus EnableIO();
  OSStatus SetupOutput(AudioDeviceID output_id);
  OSStatus SetupCallbacks();
  OSStatus SetupStreamFormats();
  void AllocateInputData();

  
  OSStatus HandleInputCallback(AudioUnitRenderActionFlags* io_action_flags,
                               const AudioTimeStamp* time_stamp,
                               UInt32 bus_number,
                               UInt32 number_of_frames,
                               AudioBufferList* io_data);

  OSStatus HandleVarispeedCallback(AudioUnitRenderActionFlags* io_action_flags,
                                   const AudioTimeStamp* time_stamp,
                                   UInt32 bus_number,
                                   UInt32 number_of_frames,
                                   AudioBufferList* io_data);

  OSStatus HandleOutputCallback(AudioUnitRenderActionFlags* io_action_flags,
                                const AudioTimeStamp* time_stamp,
                                UInt32 bus_number,
                                UInt32 number_of_frames,
                                AudioBufferList* io_data);

  
  static OSStatus InputProc(void* user_data,
                            AudioUnitRenderActionFlags* io_action_flags,
                            const AudioTimeStamp* time_stamp,
                            UInt32 bus_number,
                            UInt32 number_of_frames,
                            AudioBufferList* io_data);

  static OSStatus VarispeedProc(void* user_data,
                                AudioUnitRenderActionFlags* io_action_flags,
                                const AudioTimeStamp* time_stamp,
                                UInt32 bus_number,
                                UInt32 number_of_frames,
                                AudioBufferList* io_data);

  static OSStatus OutputProc(void* user_data,
                             AudioUnitRenderActionFlags* io_action_flags,
                             const AudioTimeStamp* time_stamp,
                             UInt32 bus_number,
                             UInt32 number_of_frames,
                             AudioBufferList*  io_data);

  
  AudioManagerMac* manager_;

  
  AudioParameters params_;

  double input_sample_rate_;
  double output_sample_rate_;

  
  AudioSourceCallback* source_;

  
  AudioDeviceID input_id_;
  AudioDeviceID output_id_;

  
  AudioBufferList* input_buffer_list_;

  
  scoped_ptr<AudioBus> input_bus_;

  
  scoped_ptr<AudioBus> wrapper_bus_;

  class AudioDeviceInfo {
   public:
    AudioDeviceInfo()
        : id_(kAudioDeviceUnknown),
          is_input_(false),
          buffer_size_frames_(0) {}
    void Initialize(AudioDeviceID inID, bool isInput);
    bool IsInitialized() const { return id_ != kAudioDeviceUnknown; }

    AudioDeviceID id_;
    bool is_input_;
    UInt32 buffer_size_frames_;
  };

  AudioDeviceInfo input_info_;
  AudioDeviceInfo output_info_;

  
  AudioFifo fifo_;

  
  int target_fifo_frames_;

  
  
  double average_delta_;

  
  double fifo_rate_compensation_;

  
  AudioUnit input_unit_;
  AudioUnit varispeed_unit_;
  AudioUnit output_unit_;

  double first_input_time_;

  bool is_running_;
  int hardware_buffer_size_;
  int channels_;

  
  
  scoped_ptr<ChannelMixer> channel_mixer_;
  scoped_ptr<AudioBus> mixer_bus_;

  DISALLOW_COPY_AND_ASSIGN(AudioSynchronizedStream);
};

}  

#endif  
