// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_SOURCE_EXTRA_DATA_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_SOURCE_EXTRA_DATA_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"
#include "content/renderer/media/media_stream_source_observer.h"
#include "content/renderer/media/webrtc_audio_capturer.h"
#include "third_party/libjingle/source/talk/app/webrtc/videosourceinterface.h"
#include "third_party/WebKit/public/platform/WebMediaStreamSource.h"

namespace content {

class CONTENT_EXPORT MediaStreamSourceExtraData
    : NON_EXPORTED_BASE(public blink::WebMediaStreamSource::ExtraData) {
 public:
  typedef base::Callback<void(const blink::WebMediaStreamSource& source)>
      SourceStopCallback;

  MediaStreamSourceExtraData(const StreamDeviceInfo& device_info,
                             const SourceStopCallback& stop_callback);
  MediaStreamSourceExtraData();
  virtual ~MediaStreamSourceExtraData();

  
  const StreamDeviceInfo& device_info() const {
    return device_info_;
  }

  void SetVideoSource(webrtc::VideoSourceInterface* source) {
    video_source_ = source;
    source_observer_.reset(new MediaStreamSourceObserver(source, this));
  }

  void SetLocalAudioSource(webrtc::AudioSourceInterface* source) {
    local_audio_source_ = source;
    
    
  }

  void SetAudioCapturer(WebRtcAudioCapturer* capturer) {
    DCHECK(!audio_capturer_);
    audio_capturer_ = capturer;
  }

  WebRtcAudioCapturer* GetAudioCapturer() const {
    
    
    
    
    
    if (audio_capturer_ &&
        device_info_.session_id == audio_capturer_->session_id()) {
      return audio_capturer_;
    }
    return NULL;
  }

  webrtc::VideoSourceInterface* video_source() { return video_source_.get(); }
  webrtc::AudioSourceInterface* local_audio_source() {
    return local_audio_source_.get();
  }

  void OnLocalSourceStop();

 private:
  StreamDeviceInfo device_info_;

  scoped_refptr<webrtc::VideoSourceInterface> video_source_;

  
  
  
  
  scoped_refptr<webrtc::AudioSourceInterface> local_audio_source_;
  scoped_ptr<MediaStreamSourceObserver> source_observer_;

  scoped_refptr<WebRtcAudioCapturer> audio_capturer_;

  SourceStopCallback stop_callback_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamSourceExtraData);
};

}  

#endif  
