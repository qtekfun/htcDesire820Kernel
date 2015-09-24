// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// No more data will be pulled from the data source, or written to the device.

#ifndef MEDIA_AUDIO_ALSA_ALSA_OUTPUT_H_
#define MEDIA_AUDIO_ALSA_ALSA_OUTPUT_H_

#include <alsa/asoundlib.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace base {
class MessageLoop;
}

namespace media {

class AlsaWrapper;
class AudioManagerBase;
class ChannelMixer;
class SeekableBuffer;

class MEDIA_EXPORT AlsaPcmOutputStream : public AudioOutputStream {
 public:
  
  
  static const char kDefaultDevice[];

  
  
  static const char kAutoSelectDevice[];

  
  static const char kPlugPrefix[];

  
  static const uint32 kMinLatencyMicros;

  
  
  
  
  
  
  
  AlsaPcmOutputStream(const std::string& device_name,
                      const AudioParameters& params,
                      AlsaWrapper* wrapper,
                      AudioManagerBase* manager);

  virtual ~AlsaPcmOutputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

 private:
  friend class AlsaPcmOutputStreamTest;
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest,
                           AutoSelectDevice_DeviceSelect);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest,
                           AutoSelectDevice_FallbackDevices);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, AutoSelectDevice_HintFail);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, BufferPacket);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, BufferPacket_Negative);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, BufferPacket_StopStream);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, BufferPacket_Underrun);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, BufferPacket_FullBuffer);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, ConstructedState);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, LatencyFloor);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, OpenClose);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, PcmOpenFailed);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, PcmSetParamsFailed);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, ScheduleNextWrite);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest,
                           ScheduleNextWrite_StopStream);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, StartStop);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, WritePacket_FinishedPacket);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, WritePacket_NormalPacket);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, WritePacket_StopStream);
  FRIEND_TEST_ALL_PREFIXES(AlsaPcmOutputStreamTest, WritePacket_WriteFails);

  
  enum InternalState {
    kInError = 0,
    kCreated,
    kIsOpened,
    kIsPlaying,
    kIsStopped,
    kIsClosed
  };
  friend std::ostream& operator<<(std::ostream& os, InternalState);

  
  
  void BufferPacket(bool* source_exhausted);
  void WritePacket();
  void WriteTask();
  void ScheduleNextWrite(bool source_exhausted);

  
  static base::TimeDelta FramesToTimeDelta(int frames, double sample_rate);
  std::string FindDeviceForChannels(uint32 channels);
  snd_pcm_sframes_t GetAvailableFrames();
  snd_pcm_sframes_t GetCurrentDelay();

  
  
  snd_pcm_t* AutoSelectDevice(uint32 latency);

  
  
  bool CanTransitionTo(InternalState to);
  InternalState TransitionTo(InternalState to);
  InternalState state();

  
  
  bool IsOnAudioThread() const;

  
  
  
  
  
  
  
  
  int RunDataCallback(AudioBus* audio_bus, AudioBuffersState buffers_state);
  void RunErrorCallback(int code);

  
  
  void set_source_callback(AudioSourceCallback* callback);

  
  
  const std::string requested_device_name_;
  const snd_pcm_format_t pcm_format_;
  const uint32 channels_;
  const ChannelLayout channel_layout_;
  const uint32 sample_rate_;
  const uint32 bytes_per_sample_;
  const uint32 bytes_per_frame_;

  
  std::string device_name_;
  uint32 packet_size_;
  base::TimeDelta latency_;
  uint32 bytes_per_output_frame_;
  uint32 alsa_buffer_frames_;

  
  
  
  bool stop_stream_;

  
  AlsaWrapper* wrapper_;

  
  AudioManagerBase* manager_;

  
  
  
  
  base::MessageLoop* message_loop_;

  
  snd_pcm_t* playback_handle_;

  scoped_ptr<media::SeekableBuffer> buffer_;
  uint32 frames_per_packet_;

  
  
  base::WeakPtrFactory<AlsaPcmOutputStream> weak_factory_;

  InternalState state_;
  float volume_;  

  AudioSourceCallback* source_callback_;

  
  scoped_ptr<AudioBus> audio_bus_;

  
  scoped_ptr<ChannelMixer> channel_mixer_;
  scoped_ptr<AudioBus> mixed_audio_bus_;

  DISALLOW_COPY_AND_ASSIGN(AlsaPcmOutputStream);
};

MEDIA_EXPORT std::ostream& operator<<(std::ostream& os,
                                      AlsaPcmOutputStream::InternalState);

};  

#endif  
