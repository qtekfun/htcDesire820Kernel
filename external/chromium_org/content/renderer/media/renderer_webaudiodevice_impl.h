// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_RENDERER_WEBAUDIODEVICE_IMPL_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_RENDERER_WEBAUDIODEVICE_IMPL_H_

#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_renderer_sink.h"
#include "third_party/WebKit/public/platform/WebAudioDevice.h"
#include "third_party/WebKit/public/platform/WebVector.h"

namespace media {
class AudioOutputDevice;
}

namespace content {

class RendererWebAudioDeviceImpl
    : public blink::WebAudioDevice,
      public media::AudioRendererSink::RenderCallback {
 public:
  RendererWebAudioDeviceImpl(const media::AudioParameters& params,
                             blink::WebAudioDevice::RenderCallback* callback,
                             int session_id);
  virtual ~RendererWebAudioDeviceImpl();

  
  virtual void start();
  virtual void stop();
  virtual double sampleRate();

  
  virtual int Render(media::AudioBus* dest,
                     int audio_delay_milliseconds) OVERRIDE;

  virtual void RenderIO(media::AudioBus* source,
                        media::AudioBus* dest,
                        int audio_delay_milliseconds) OVERRIDE;

  virtual void OnRenderError() OVERRIDE;

 private:
  const media::AudioParameters params_;

  
  blink::WebAudioDevice::RenderCallback* const client_callback_;

  
  
  base::ThreadChecker thread_checker_;

  
  scoped_refptr<media::AudioOutputDevice> output_device_;

  
  int session_id_;

  DISALLOW_COPY_AND_ASSIGN(RendererWebAudioDeviceImpl);
};

}  

#endif  
