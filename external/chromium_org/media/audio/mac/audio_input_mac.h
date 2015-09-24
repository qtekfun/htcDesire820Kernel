// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_MAC_AUDIO_INPUT_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_INPUT_MAC_H_

#include <AudioToolbox/AudioFormat.h>
#include <AudioToolbox/AudioQueue.h>

#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerBase;

class PCMQueueInAudioInputStream : public AudioInputStream {
 public:
  
  PCMQueueInAudioInputStream(AudioManagerBase* manager,
                             const AudioParameters& params);
  virtual ~PCMQueueInAudioInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE;
  virtual bool GetAutomaticGainControl() OVERRIDE;

 private:
  
  void HandleError(OSStatus err);

  
  bool SetupBuffers();

  
  OSStatus QueueNextBuffer(AudioQueueBufferRef audio_buffer);

  
  static void HandleInputBufferStatic(
      void* data,
      AudioQueueRef audio_queue,
      AudioQueueBufferRef audio_buffer,
      const AudioTimeStamp* start_time,
      UInt32 num_packets,
      const AudioStreamPacketDescription* desc);

  
  void HandleInputBuffer(AudioQueueRef audio_queue,
                         AudioQueueBufferRef audio_buffer,
                         const AudioTimeStamp* start_time,
                         UInt32 num_packets,
                         const AudioStreamPacketDescription* packet_desc);

  static const int kNumberBuffers = 3;

  
  AudioManagerBase* manager_;
  
  AudioInputCallback* callback_;
  
  AudioStreamBasicDescription format_;
  
  AudioQueueRef audio_queue_;
  
  uint32 buffer_size_bytes_;
  
  bool started_;
  
  base::TimeTicks last_fill_;

  DISALLOW_COPY_AND_ASSIGN(PCMQueueInAudioInputStream);
};

}  

#endif  
