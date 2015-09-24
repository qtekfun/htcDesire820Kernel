// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_ANDROID_OPENSLES_INPUT_H_
#define MEDIA_AUDIO_ANDROID_OPENSLES_INPUT_H_

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

class OpenSLESInputStream : public AudioInputStream {
 public:
  static const int kMaxNumOfBuffersInQueue = 2;

  OpenSLESInputStream(AudioManagerAndroid* manager,
                      const AudioParameters& params);

  virtual ~OpenSLESInputStream();

  
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
  bool CreateRecorder();

  
  static void SimpleBufferQueueCallback(
      SLAndroidSimpleBufferQueueItf buffer_queue,
      void* instance);

  
  void ReadBufferQueue();

  
  void SetupAudioBuffer();

  
  void ReleaseAudioBuffer();

  
  
  void HandleError(SLresult error);

  base::ThreadChecker thread_checker_;

  
  
  base::Lock lock_;

  AudioManagerAndroid* audio_manager_;

  AudioInputCallback* callback_;

  
  media::ScopedSLObjectItf recorder_object_;
  media::ScopedSLObjectItf engine_object_;

  SLRecordItf recorder_;

  
  SLAndroidSimpleBufferQueueItf simple_buffer_queue_;

  SLDataFormat_PCM format_;

  
  
  uint8* audio_data_[kMaxNumOfBuffersInQueue];

  int active_buffer_index_;
  int buffer_size_bytes_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(OpenSLESInputStream);
};

}  

#endif  
