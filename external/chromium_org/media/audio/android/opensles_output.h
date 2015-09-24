// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_ANDROID_OPENSLES_OUTPUT_H_
#define MEDIA_AUDIO_ANDROID_OPENSLES_OUTPUT_H_

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "media/audio/android/opensles_util.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerAndroid;

class OpenSLESOutputStream : public AudioOutputStream {
 public:
  static const int kMaxNumOfBuffersInQueue = 2;

  OpenSLESOutputStream(AudioManagerAndroid* manager,
                       const AudioParameters& params);

  virtual ~OpenSLESOutputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

  
  
  void SetMute(bool muted);

 private:
  bool CreatePlayer();

  
  static void SimpleBufferQueueCallback(
      SLAndroidSimpleBufferQueueItf buffer_queue,
      void* instance);

  
  
  void FillBufferQueue();

  
  void FillBufferQueueNoLock();

  
  void SetupAudioBuffer();

  
  void ReleaseAudioBuffer();

  
  
  void HandleError(SLresult error);

  base::ThreadChecker thread_checker_;

  
  
  base::Lock lock_;

  AudioManagerAndroid* audio_manager_;

  AudioSourceCallback* callback_;

  
  media::ScopedSLObjectItf engine_object_;
  media::ScopedSLObjectItf player_object_;
  media::ScopedSLObjectItf output_mixer_;

  SLPlayItf player_;

  
  SLAndroidSimpleBufferQueueItf simple_buffer_queue_;

  SLDataFormat_PCM format_;

  
  
  uint8* audio_data_[kMaxNumOfBuffersInQueue];

  int active_buffer_index_;
  size_t buffer_size_bytes_;

  bool started_;

  
  
  
  
  bool muted_;

  
  float volume_;

  
  scoped_ptr<AudioBus> audio_bus_;

  DISALLOW_COPY_AND_ASSIGN(OpenSLESOutputStream);
};

}  

#endif  
