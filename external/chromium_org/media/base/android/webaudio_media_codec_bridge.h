// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_WEBAUDIO_MEDIA_CODEC_BRIDGE_H_
#define MEDIA_BASE_ANDROID_WEBAUDIO_MEDIA_CODEC_BRIDGE_H_

#include <jni.h>

#include "base/file_descriptor_posix.h"
#include "base/memory/shared_memory.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT WebAudioMediaCodecBridge {
 public:
  
  
  // information and decoded PCM samples are written to |pcm_output|.
  
  WebAudioMediaCodecBridge(base::SharedMemoryHandle encoded_audio_handle,
                           base::FileDescriptor pcm_output,
                           uint32_t data_size);
  ~WebAudioMediaCodecBridge();

  
  
  static bool RegisterWebAudioMediaCodecBridge(JNIEnv* env);

  
  
  static void RunWebAudioMediaCodec(
      base::SharedMemoryHandle encoded_audio_handle,
      base::FileDescriptor pcm_output,
      uint32_t data_size);

  void OnChunkDecoded(JNIEnv* env,
                      jobject ,
                      jobject buf,
                      jint buf_size,
                      jint input_channel_count,
                      jint output_channel_count);

  void InitializeDestination(JNIEnv* env,
                             jobject ,
                             jint channel_count,
                             jint sample_rate,
                             jlong duration_us);

 private:
  
  
  
  bool DecodeInMemoryAudioFile();

  
  
  
  int SaveEncodedAudioToFile(JNIEnv*, jobject);

  
  
  base::SharedMemoryHandle encoded_audio_handle_;

  // The audio file information and decoded pcm data are written to
  
  int pcm_output_;

  
  uint32_t data_size_;

  DISALLOW_COPY_AND_ASSIGN(WebAudioMediaCodecBridge);
};

}  
#endif  
