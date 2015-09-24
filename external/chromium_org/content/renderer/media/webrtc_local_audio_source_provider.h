// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_LOCAL_AUDIO_SOURCE_PROVIDER_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_LOCAL_AUDIO_SOURCE_PROVIDER_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "content/public/renderer/media_stream_audio_sink.h"
#include "media/base/audio_converter.h"
#include "third_party/WebKit/public/platform/WebAudioSourceProvider.h"
#include "third_party/WebKit/public/platform/WebVector.h"

namespace media {
class AudioBus;
class AudioConverter;
class AudioFifo;
class AudioParameters;
}

namespace blink {
class WebAudioSourceProviderClient;
}

namespace content {

class CONTENT_EXPORT WebRtcLocalAudioSourceProvider
    :  NON_EXPORTED_BASE(public blink::WebAudioSourceProvider),
       NON_EXPORTED_BASE(public media::AudioConverter::InputCallback),
       NON_EXPORTED_BASE(public MediaStreamAudioSink) {
 public:
  static const size_t kWebAudioRenderBufferSize;

  WebRtcLocalAudioSourceProvider();
  virtual ~WebRtcLocalAudioSourceProvider();

  
  virtual void OnData(const int16* audio_data,
                      int sample_rate,
                      int number_of_channels,
                      int number_of_frames) OVERRIDE;
  virtual void OnSetFormat(const media::AudioParameters& params) OVERRIDE;

  
  virtual void setClient(blink::WebAudioSourceProviderClient* client) OVERRIDE;
  virtual void provideInput(const blink::WebVector<float*>& audio_data,
                            size_t number_of_frames) OVERRIDE;

  
  
  
  virtual double ProvideInput(media::AudioBus* audio_bus,
                              base::TimeDelta buffer_delay) OVERRIDE;

  
  
  
  
  
  
  
  void SetSinkParamsForTesting(const media::AudioParameters& sink_params);

 private:
  
  base::ThreadChecker capture_thread_checker_;

  scoped_ptr<media::AudioConverter> audio_converter_;
  scoped_ptr<media::AudioFifo> fifo_;
  scoped_ptr<media::AudioBus> input_bus_;
  scoped_ptr<media::AudioBus> output_wrapper_;
  bool is_enabled_;
  media::AudioParameters source_params_;
  media::AudioParameters sink_params_;

  
  base::Lock lock_;

  
  base::TimeTicks last_fill_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcLocalAudioSourceProvider);
};

}  

#endif  
