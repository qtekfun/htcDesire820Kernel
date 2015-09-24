// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBAUDIOSOURCEPROVIDER_IMPL_H_
#define CONTENT_RENDERER_MEDIA_WEBAUDIOSOURCEPROVIDER_IMPL_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "media/base/audio_renderer_sink.h"
#include "third_party/WebKit/public/platform/WebAudioSourceProvider.h"
#include "third_party/WebKit/public/platform/WebVector.h"

namespace blink {
class WebAudioSourceProviderClient;
}

namespace content {

class CONTENT_EXPORT WebAudioSourceProviderImpl
    : NON_EXPORTED_BASE(public blink::WebAudioSourceProvider),
      NON_EXPORTED_BASE(public media::AudioRendererSink) {
 public:
  explicit WebAudioSourceProviderImpl(
      const scoped_refptr<media::AudioRendererSink>& sink);

  
  virtual void setClient(blink::WebAudioSourceProviderClient* client);
  virtual void provideInput(const blink::WebVector<float*>& audio_data,
                            size_t number_of_frames);

  
  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Play() OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual bool SetVolume(double volume) OVERRIDE;
  virtual void Initialize(const media::AudioParameters& params,
                          RenderCallback* renderer) OVERRIDE;

 protected:
  virtual ~WebAudioSourceProviderImpl();

 private:
  
  void OnSetFormat();

  base::WeakPtrFactory<WebAudioSourceProviderImpl> weak_this_;

  
  base::Closure set_format_cb_;

  
  int channels_;
  int sample_rate_;
  double volume_;

  
  enum PlaybackState { kStopped, kStarted, kPlaying };
  PlaybackState state_;

  
  media::AudioRendererSink::RenderCallback* renderer_;

  
  blink::WebAudioSourceProviderClient* client_;

  
  base::Lock sink_lock_;
  scoped_refptr<media::AudioRendererSink> sink_;
  scoped_ptr<media::AudioBus> bus_wrapper_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(WebAudioSourceProviderImpl);
};

}  

#endif  
