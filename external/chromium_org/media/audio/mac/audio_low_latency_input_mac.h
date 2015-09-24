// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef MEDIA_AUDIO_MAC_AUDIO_LOW_LATENCY_INPUT_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_LOW_LATENCY_INPUT_MAC_H_

#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>

#include "base/atomicops.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "media/audio/agc_audio_stream.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/seekable_buffer.h"

namespace media {

class AudioManagerMac;
class DataBuffer;

class AUAudioInputStream : public AgcAudioStream<AudioInputStream> {
 public:
  
  
  AUAudioInputStream(AudioManagerMac* manager,
                     const AudioParameters& input_params,
                     const AudioParameters& output_params,
                     AudioDeviceID audio_device_id);
  
  
  virtual ~AUAudioInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;

  
  MEDIA_EXPORT static int HardwareSampleRate();

  bool started() const { return started_; }
  AudioUnit audio_unit() { return audio_unit_; }
  AudioBufferList* audio_buffer_list() { return &audio_buffer_list_; }

 private:
  
  static OSStatus InputProc(void* user_data,
                            AudioUnitRenderActionFlags* flags,
                            const AudioTimeStamp* time_stamp,
                            UInt32 bus_number,
                            UInt32 number_of_frames,
                            AudioBufferList* io_data);

  
  OSStatus Provide(UInt32 number_of_frames, AudioBufferList* io_data,
                   const AudioTimeStamp* time_stamp);

  
  
  double GetHardwareLatency();

  
  double GetCaptureLatency(const AudioTimeStamp* input_time_stamp);

  
  int GetNumberOfChannelsFromStream();

  
  void HandleError(OSStatus err);

  
  
  bool IsVolumeSettableOnChannel(int channel);

  
  AudioManagerMac* manager_;

  
  size_t number_of_frames_;

  
  AudioInputCallback* sink_;

  
  
  AudioStreamBasicDescription format_;

  
  
  
  AudioUnit audio_unit_;

  
  AudioDeviceID input_device_id_;

  
  AudioBufferList audio_buffer_list_;

  
  
  scoped_ptr<uint8[]> audio_data_buffer_;

  
  bool started_;

  
  double hardware_latency_frames_;

  
  int fifo_delay_bytes_;

  
  
  int number_of_channels_in_frame_;

  
  scoped_ptr<media::SeekableBuffer> fifo_;

   
   
  scoped_refptr<media::DataBuffer> data_;

  
  
  int requested_size_bytes_;

  DISALLOW_COPY_AND_ASSIGN(AUAudioInputStream);
};

}  

#endif  
