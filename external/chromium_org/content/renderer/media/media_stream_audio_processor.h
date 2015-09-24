// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_AUDIO_PROCESSOR_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_AUDIO_PROCESSOR_H_

#include "base/atomicops.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "media/base/audio_converter.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediaconstraintsinterface.h"
#include "third_party/webrtc/modules/audio_processing/include/audio_processing.h"
#include "third_party/webrtc/modules/interface/module_common_types.h"

namespace media {
class AudioBus;
class AudioFifo;
class AudioParameters;
}  

namespace webrtc {
class AudioFrame;
}

namespace content {

class CONTENT_EXPORT MediaStreamAudioProcessor {
 public:
  explicit MediaStreamAudioProcessor(
      const webrtc::MediaConstraintsInterface* constraints);
  ~MediaStreamAudioProcessor();

  
  
  void PushCaptureData(media::AudioBus* audio_source);

  
  
  
  
  
  void PushRenderData(const int16* render_audio,
                      int sample_rate,
                      int number_of_channels,
                      int number_of_frames,
                      base::TimeDelta render_delay);

  
  
  
  
  
  
  
  
  
  
  bool ProcessAndConsumeData(base::TimeDelta capture_delay,
                             int volume,
                             bool key_pressed,
                             int16** out);

  
  
  
  void SetCaptureFormat(const media::AudioParameters& source_params);

  
  const media::AudioParameters& OutputFormat() const;

  
  bool has_audio_processing() const { return audio_processing_.get() != NULL; }

 private:
  class MediaStreamAudioConverter;

  
  void InitializeAudioProcessingModule(
      const webrtc::MediaConstraintsInterface* constraints);

  
  void InitializeRenderConverterIfNeeded(int sample_rate,
                                         int number_of_channels,
                                         int frames_per_buffer);

  
  void ProcessData(webrtc::AudioFrame* audio_frame,
                   base::TimeDelta capture_delay,
                   int volume,
                   bool key_pressed);

  
  void StopAudioProcessing();

  
  
  base::subtle::Atomic32 render_delay_ms_;

  
  
  scoped_ptr<webrtc::AudioProcessing> audio_processing_;

  
  scoped_ptr<MediaStreamAudioConverter> capture_converter_;

  
  webrtc::AudioFrame capture_frame_;

  
  
  scoped_ptr<MediaStreamAudioConverter> render_converter_;

  
  webrtc::AudioFrame render_frame_;

  
  scoped_ptr<media::AudioBus> render_data_bus_;

  
  base::ThreadChecker main_thread_checker_;

  
  base::ThreadChecker capture_thread_checker_;

  
  base::ThreadChecker render_thread_checker_;
};

}  

#endif  
