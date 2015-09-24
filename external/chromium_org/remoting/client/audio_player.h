// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_AUDIO_PLAYER_H_
#define REMOTING_CLIENT_AUDIO_PLAYER_H_

#include <list>

#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "remoting/proto/audio.pb.h"

namespace remoting {

class AudioPlayer {
 public:
  virtual ~AudioPlayer();

  void ProcessAudioPacket(scoped_ptr<AudioPacket> packet);

 protected:
  AudioPlayer();

  
  virtual uint32 GetSamplesPerFrame() = 0;

  
  
  virtual bool ResetAudioPlayer(AudioPacket::SamplingRate sampling_rate) = 0;

  
  static void AudioPlayerCallback(void* samples,
                                  uint32 buffer_size,
                                  void* data);

 private:
  friend class AudioPlayerTest;

  typedef std::list<AudioPacket*> AudioPacketQueue;

  void ResetQueue();
  void FillWithSamples(void* samples, uint32 buffer_size);

  AudioPacket::SamplingRate sampling_rate_;

  bool start_failed_;

  
  
  
  base::Lock lock_;

  AudioPacketQueue queued_packets_;
  int queued_bytes_;

  
  size_t bytes_consumed_;

  DISALLOW_COPY_AND_ASSIGN(AudioPlayer);
};

}  

#endif  
