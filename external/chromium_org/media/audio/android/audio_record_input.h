// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_ANDROID_AUDIO_RECORD_INPUT_H_
#define MEDIA_AUDIO_ANDROID_AUDIO_RECORD_INPUT_H_

#include "base/android/jni_android.h"
#include "base/threading/thread_checker.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerAndroid;

class MEDIA_EXPORT AudioRecordInputStream : public AudioInputStream {
 public:
  AudioRecordInputStream(AudioManagerAndroid* manager,
                         const AudioParameters& params);

  virtual ~AudioRecordInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE;
  virtual bool GetAutomaticGainControl() OVERRIDE;

  static bool RegisterAudioRecordInput(JNIEnv* env);

  
  void OnData(JNIEnv* env, jobject obj, jint size, jint hardware_delay_bytes);

  
  
  void CacheDirectBufferAddress(JNIEnv* env, jobject obj, jobject byte_buffer);

 private:
  base::ThreadChecker thread_checker_;
  AudioManagerAndroid* audio_manager_;

  
  base::android::ScopedJavaGlobalRef<jobject> j_audio_record_;

  
  
  
  AudioInputCallback* callback_;

  
  uint8* direct_buffer_address_;

  DISALLOW_COPY_AND_ASSIGN(AudioRecordInputStream);
};

}  

#endif  
