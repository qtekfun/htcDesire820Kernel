// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBAUDIO_CAPTURER_SOURCE_H_
#define CONTENT_RENDERER_MEDIA_WEBAUDIO_CAPTURER_SOURCE_H_

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_capturer_source.h"
#include "media/base/audio_fifo.h"
#include "third_party/WebKit/public/platform/WebAudioDestinationConsumer.h"
#include "third_party/WebKit/public/platform/WebVector.h"

namespace content {

class WebRtcAudioCapturer;
class WebRtcLocalAudioTrack;

class WebAudioCapturerSource
    :  public base::RefCountedThreadSafe<WebAudioCapturerSource>,
       public blink::WebAudioDestinationConsumer {
 public:
  WebAudioCapturerSource();

  
  
  virtual void setFormat(size_t number_of_channels, float sample_rate) OVERRIDE;
  
  
  virtual void consumeAudio(const blink::WebVector<const float*>& audio_data,
      size_t number_of_frames) OVERRIDE;

  
  
  
  
  void Start(WebRtcLocalAudioTrack* track, WebRtcAudioCapturer* capturer);

  
  void Stop();

 protected:
  friend class base::RefCountedThreadSafe<WebAudioCapturerSource>;
  virtual ~WebAudioCapturerSource();

 private:
  
  base::ThreadChecker thread_checker_;

  
  
  
  WebRtcLocalAudioTrack* track_;

  
  
  
  WebRtcAudioCapturer* capturer_;

  media::AudioParameters params_;

  
  bool audio_format_changed_;

  
  scoped_ptr<media::AudioBus> wrapper_bus_;

  
  scoped_ptr<media::AudioBus> capture_bus_;

  
  scoped_ptr<media::AudioFifo> fifo_;

  
  base::Lock lock_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(WebAudioCapturerSource);
};

}  

#endif  
