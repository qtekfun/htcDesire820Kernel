// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_LOCAL_AUDIO_TRACK_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_LOCAL_AUDIO_TRACK_H_

#include <list>
#include <string>

#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "content/renderer/media/media_stream_audio_track_sink.h"
#include "content/renderer/media/tagged_list.h"
#include "content/renderer/media/webrtc_audio_device_impl.h"
#include "content/renderer/media/webrtc_local_audio_source_provider.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediaconstraintsinterface.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediastreaminterface.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediastreamtrack.h"
#include "third_party/libjingle/source/talk/media/base/audiorenderer.h"

namespace cricket {
class AudioRenderer;
}  

namespace media {
class AudioBus;
}  

namespace content {

class MediaStreamAudioSink;
class MediaStreamAudioSinkOwner;
class PeerConnectionAudioSink;
class WebAudioCapturerSource;
class WebRtcAudioCapturer;

class CONTENT_EXPORT WebRtcLocalAudioTrack
    : NON_EXPORTED_BASE(public cricket::AudioRenderer),
      NON_EXPORTED_BASE(
          public webrtc::MediaStreamTrack<webrtc::AudioTrackInterface>) {
 public:
  static scoped_refptr<WebRtcLocalAudioTrack> Create(
      const std::string& id,
      const scoped_refptr<WebRtcAudioCapturer>& capturer,
      WebAudioCapturerSource* webaudio_source,
      webrtc::AudioSourceInterface* track_source,
      const webrtc::MediaConstraintsInterface* constraints);

  
  
  
  void AddSink(MediaStreamAudioSink* sink);

  
  
  void RemoveSink(MediaStreamAudioSink* sink);

  
  
  
  void AddSink(PeerConnectionAudioSink* sink);
  void RemoveSink(PeerConnectionAudioSink* sink);

  
  
  void Start();

  
  
  void Stop();

  
  
  void Capture(media::AudioBus* audio_source,
               int audio_delay_milliseconds,
               int volume,
               bool key_pressed);

  
  
  
  void OnSetFormat(const media::AudioParameters& params);

  blink::WebAudioSourceProvider* audio_source_provider() const {
    return source_provider_.get();
  }

 protected:
  WebRtcLocalAudioTrack(
      const std::string& label,
      const scoped_refptr<WebRtcAudioCapturer>& capturer,
      WebAudioCapturerSource* webaudio_source,
      webrtc::AudioSourceInterface* track_source,
      const webrtc::MediaConstraintsInterface* constraints);

  virtual ~WebRtcLocalAudioTrack();

 private:
  typedef TaggedList<MediaStreamAudioTrackSink> SinkList;

  
  virtual void AddChannel(int channel_id) OVERRIDE;
  virtual void RemoveChannel(int channel_id) OVERRIDE;

  
  virtual webrtc::AudioSourceInterface* GetSource() const OVERRIDE;
  virtual cricket::AudioRenderer* GetRenderer() OVERRIDE;

  
  virtual std::string kind() const OVERRIDE;

  
  
  scoped_refptr<WebRtcAudioCapturer> capturer_;

  
  
  scoped_refptr<WebAudioCapturerSource> webaudio_source_;

  
  
  talk_base::scoped_refptr<webrtc::AudioSourceInterface> track_source_;

  
  
  
  SinkList sinks_;

  
  base::ThreadChecker main_render_thread_checker_;

  
  base::ThreadChecker capture_thread_checker_;

  
  mutable base::Lock lock_;

  
  std::vector<int> voe_channels_;

  bool need_audio_processing_;

  
  
  class ConfiguredBuffer;
  scoped_ptr<ConfiguredBuffer> buffer_;

  
  
  scoped_ptr<WebRtcLocalAudioSourceProvider> source_provider_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcLocalAudioTrack);
};

}  

#endif  
