// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_LOCAL_AUDIO_RENDERER_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_LOCAL_AUDIO_RENDERER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "content/common/content_export.h"
#include "content/public/renderer/media_stream_audio_sink.h"
#include "content/renderer/media/media_stream_audio_renderer.h"
#include "content/renderer/media/webrtc_audio_device_impl.h"
#include "content/renderer/media/webrtc_local_audio_track.h"
#include "third_party/WebKit/public/platform/WebMediaStreamTrack.h"

namespace media {
class AudioBus;
class AudioFifo;
class AudioOutputDevice;
class AudioParameters;
}

namespace content {

class WebRtcAudioCapturer;

class CONTENT_EXPORT WebRtcLocalAudioRenderer
    : NON_EXPORTED_BASE(public MediaStreamAudioRenderer),
      NON_EXPORTED_BASE(public MediaStreamAudioSink),
      NON_EXPORTED_BASE(public media::AudioRendererSink::RenderCallback) {
 public:
  
  
  
  WebRtcLocalAudioRenderer(const blink::WebMediaStreamTrack& audio_track,
                           int source_render_view_id,
                           int session_id,
                           int frames_per_buffer);

  
  
  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Play() OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual void SetVolume(float volume) OVERRIDE;
  virtual base::TimeDelta GetCurrentRenderTime() const OVERRIDE;
  virtual bool IsLocalRenderer() const OVERRIDE;

  const base::TimeDelta& total_render_time() const {
    return total_render_time_;
  }

 protected:
  virtual ~WebRtcLocalAudioRenderer();

 private:
  

  
  virtual void OnData(const int16* audio_data,
                      int sample_rate,
                      int number_of_channels,
                      int number_of_frames) OVERRIDE;

  
  virtual void OnSetFormat(const media::AudioParameters& params) OVERRIDE;

  
  
  
  virtual int Render(media::AudioBus* audio_bus,
                     int audio_delay_milliseconds) OVERRIDE;
  virtual void OnRenderError() OVERRIDE;

  
  
  
  
  void MaybeStartSink();

  
  void ReconfigureSink(const media::AudioParameters& params);

  
  
  
  
  
  
  
  
  blink::WebMediaStreamTrack audio_track_;

  
  const int source_render_view_id_;
  const int session_id_;

  
  
  const scoped_refptr<base::MessageLoopProxy> message_loop_;

  
  scoped_refptr<media::AudioOutputDevice> sink_;

  
  scoped_ptr<media::AudioFifo> loopback_fifo_;

  
  
  
  base::TimeTicks last_render_time_;

  
  base::TimeDelta total_render_time_;

  
  media::AudioParameters source_params_;

  
  media::AudioParameters sink_params_;

  
  bool playing_;

  
  mutable base::Lock thread_lock_;

  
  const int frames_per_buffer_;

  
  
  const std::string output_device_id_;

  
  float volume_;

  
  bool sink_started_;

  
  base::ThreadChecker capture_thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcLocalAudioRenderer);
};

}  

#endif  
