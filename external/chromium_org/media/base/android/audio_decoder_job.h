// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_AUDIO_DECODER_JOB_H_
#define MEDIA_BASE_ANDROID_AUDIO_DECODER_JOB_H_

#include <jni.h>

#include "media/base/android/media_decoder_job.h"

namespace media {

class AudioCodecBridge;

class AudioDecoderJob : public MediaDecoderJob {
 public:
  virtual ~AudioDecoderJob();

  
  
  
  
  
  
  
  
  
  static AudioDecoderJob* Create(
      const AudioCodec audio_codec, int sample_rate, int channel_count,
      const uint8* extra_data, size_t extra_data_size, jobject media_crypto,
      const base::Closure& request_data_cb);

  void SetVolume(double volume);

 private:
  AudioDecoderJob(scoped_ptr<AudioCodecBridge> audio_decoder_bridge,
                  const base::Closure& request_data_cb);

  
  virtual void ReleaseOutputBuffer(
      int output_buffer_index,
      size_t size,
      bool render_output,
      const ReleaseOutputCompletionCallback& callback) OVERRIDE;

  virtual bool ComputeTimeToRender() const OVERRIDE;

  scoped_ptr<AudioCodecBridge> audio_codec_bridge_;
};

}  

#endif  
