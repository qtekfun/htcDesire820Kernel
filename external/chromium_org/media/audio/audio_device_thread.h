// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_DEVICE_THREAD_H_
#define MEDIA_AUDIO_AUDIO_DEVICE_THREAD_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "base/synchronization/lock.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

namespace base {
class MessageLoop;
}

namespace media {
class AudioBus;

class MEDIA_EXPORT AudioDeviceThread {
 public:
  
  
  
  class Callback {
   public:
    Callback(const AudioParameters& audio_parameters,
             base::SharedMemoryHandle memory,
             int memory_length,
             int total_segments);
    virtual ~Callback();

    
    void InitializeOnAudioThread();

    
    
    virtual void MapSharedMemory() = 0;

    
    virtual void Process(int pending_data) = 0;

   protected:
    
    
    
    const AudioParameters audio_parameters_;
    const int samples_per_ms_;
    const int bytes_per_ms_;

    base::SharedMemory shared_memory_;
    const int memory_length_;
    const int total_segments_;
    int segment_length_;

   private:
    DISALLOW_COPY_AND_ASSIGN(Callback);
  };

  AudioDeviceThread();
  ~AudioDeviceThread();

  
  
  
  void Start(AudioDeviceThread::Callback* callback,
             base::SyncSocket::Handle socket,
             const char* thread_name,
             bool synchronized_buffers);

  
  
  
  
  
  
  
  
  
  void Stop(base::MessageLoop* loop_for_join);

  
  bool IsStopped();

 private:
  
  
  
  
  
  
  
  class Thread;

  base::Lock thread_lock_;
  scoped_refptr<AudioDeviceThread::Thread> thread_;

  DISALLOW_COPY_AND_ASSIGN(AudioDeviceThread);
};

}  

#endif  
