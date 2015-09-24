// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_MEDIA_CODEC_BRIDGE_H_
#define MEDIA_BASE_ANDROID_MEDIA_CODEC_BRIDGE_H_

#include <jni.h>
#include <string>

#include "base/android/scoped_java_ref.h"
#include "base/time/time.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/video_decoder_config.h"
#include "ui/gfx/size.h"

namespace media {

struct SubsampleEntry;

enum MediaCodecStatus {
  MEDIA_CODEC_OK,
  MEDIA_CODEC_DEQUEUE_INPUT_AGAIN_LATER,
  MEDIA_CODEC_DEQUEUE_OUTPUT_AGAIN_LATER,
  MEDIA_CODEC_OUTPUT_BUFFERS_CHANGED,
  MEDIA_CODEC_OUTPUT_FORMAT_CHANGED,
  MEDIA_CODEC_INPUT_END_OF_STREAM,
  MEDIA_CODEC_OUTPUT_END_OF_STREAM,
  MEDIA_CODEC_NO_KEY,
  MEDIA_CODEC_STOPPED,
  MEDIA_CODEC_ERROR
};

enum MediaCodecDirection {
  MEDIA_CODEC_DECODER,
  MEDIA_CODEC_ENCODER,
};

class MEDIA_EXPORT MediaCodecBridge {
 public:
  
  static bool IsAvailable();

  
  static bool SupportsSetParameters();

  
  
  static bool CanDecode(const std::string& codec, bool is_secure);

  
  
  
  struct CodecsInfo {
    std::string codecs;  
    std::string name;    
    MediaCodecDirection direction;
  };

  
  static std::vector<CodecsInfo> GetCodecsInfo();

  virtual ~MediaCodecBridge();

  
  
  
  
  
  
  MediaCodecStatus Reset();

  
  
  
  
  
  void Stop();

  
  
  void GetOutputFormat(int* width, int* height);

  
  int GetInputBuffersCount();

  
  
  
  
  MediaCodecStatus QueueInputBuffer(int index,
                                    const uint8* data,
                                    size_t data_size,
                                    const base::TimeDelta& presentation_time);

  
  
  
  
  MediaCodecStatus QueueSecureInputBuffer(
      int index,
      const uint8* data,
      size_t data_size,
      const uint8* key_id,
      int key_id_size,
      const uint8* iv,
      int iv_size,
      const SubsampleEntry* subsamples,
      int subsamples_size,
      const base::TimeDelta& presentation_time);

  
  void QueueEOS(int input_buffer_index);

  
  
  
  
  
  
  MediaCodecStatus DequeueInputBuffer(const base::TimeDelta& timeout,
                                      int* index);

  
  
  
  
  
  
  
  
  
  MediaCodecStatus DequeueOutputBuffer(const base::TimeDelta& timeout,
                                       int* index,
                                       size_t* offset,
                                       size_t* size,
                                       base::TimeDelta* presentation_time,
                                       bool* end_of_stream,
                                       bool* key_frame);

  
  
  void ReleaseOutputBuffer(int index, bool render);

  
  int GetOutputBuffersCount();

  
  size_t GetOutputBuffersCapacity();

  
  
  
  bool GetOutputBuffers() WARN_UNUSED_RESULT;

  
  void GetInputBuffer(int input_buffer_index, uint8** data, size_t* capacity);

  
  
  bool CopyFromOutputBuffer(int index, size_t offset, void* dst, int dst_size);

  static bool RegisterMediaCodecBridge(JNIEnv* env);

 protected:
  
  
  static bool IsKnownUnaccelerated(const std::string& mime_type,
                                   MediaCodecDirection direction);

  MediaCodecBridge(const std::string& mime,
                   bool is_secure,
                   MediaCodecDirection direction);

  
  
  
  bool StartInternal() WARN_UNUSED_RESULT;

  jobject media_codec() { return j_media_codec_.obj(); }
  MediaCodecDirection direction_;

 private:
  
  
  bool FillInputBuffer(int index,
                       const uint8* data,
                       size_t data_size) WARN_UNUSED_RESULT;

  
  base::android::ScopedJavaGlobalRef<jobject> j_media_codec_;

  DISALLOW_COPY_AND_ASSIGN(MediaCodecBridge);
};

class AudioCodecBridge : public MediaCodecBridge {
 public:
  
  
  static AudioCodecBridge* Create(const AudioCodec& codec);

  
  static bool IsKnownUnaccelerated(const AudioCodec& codec);

  
  bool Start(const AudioCodec& codec, int sample_rate, int channel_count,
             const uint8* extra_data, size_t extra_data_size,
             bool play_audio, jobject media_crypto) WARN_UNUSED_RESULT;

  
  
  void PlayOutputBuffer(int index, size_t size);

  
  void SetVolume(double volume);

 private:
  explicit AudioCodecBridge(const std::string& mime);

  
  bool ConfigureMediaFormat(jobject j_format, const AudioCodec& codec,
                            const uint8* extra_data, size_t extra_data_size);
};

class MEDIA_EXPORT VideoCodecBridge : public MediaCodecBridge {
 public:
  
  static bool IsKnownUnaccelerated(const VideoCodec& codec,
                                   MediaCodecDirection direction);

  
  static VideoCodecBridge* CreateDecoder(
      const VideoCodec& codec,  
      bool is_secure,
      const gfx::Size& size,  
      jobject surface,        
      jobject media_crypto);  

  
  static VideoCodecBridge* CreateEncoder(
      const VideoCodec& codec,  
      const gfx::Size& size,    
      int bit_rate,             
      int frame_rate,           
      int i_frame_interval,     
      int color_format);        

  void SetVideoBitrate(int bps);
  void RequestKeyFrameSoon();

 private:
  VideoCodecBridge(const std::string& mime,
                   bool is_secure,
                   MediaCodecDirection direction);
};

}  

#endif  
