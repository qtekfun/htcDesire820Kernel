// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_AUDIO_RENDERER_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_AUDIO_RENDERER_H_

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "content/renderer/media/media_stream_audio_renderer.h"
#include "content/renderer/media/webrtc_audio_device_impl.h"
#include "media/base/audio_decoder.h"
#include "media/base/audio_pull_fifo.h"
#include "media/base/audio_renderer_sink.h"
#include "media/base/channel_layout.h"

namespace media {
class AudioOutputDevice;
}

namespace content {

class WebRtcAudioRendererSource;

class CONTENT_EXPORT WebRtcAudioRenderer
    : NON_EXPORTED_BASE(public media::AudioRendererSink::RenderCallback),
      NON_EXPORTED_BASE(public MediaStreamAudioRenderer) {
 public:
  WebRtcAudioRenderer(int source_render_view_id,
                      int session_id,
                      int sample_rate,
                      int frames_per_buffer);

  
  
  bool Initialize(WebRtcAudioRendererSource* source);

  
  
  
  
  
  
  
  
  
  scoped_refptr<MediaStreamAudioRenderer> CreateSharedAudioRendererProxy();

  
  bool IsStarted() const;

 private:
  
  
  
  virtual void Start() OVERRIDE;
  virtual void Play() OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(float volume) OVERRIDE;
  virtual base::TimeDelta GetCurrentRenderTime() const OVERRIDE;
  virtual bool IsLocalRenderer() const OVERRIDE;

 protected:
  virtual ~WebRtcAudioRenderer();

 private:
  enum State {
    UNINITIALIZED,
    PLAYING,
    PAUSED,
  };

  
  base::ThreadChecker thread_checker_;

  
  State state_;

  
  
  virtual int Render(media::AudioBus* audio_bus,
                     int audio_delay_milliseconds) OVERRIDE;
  virtual void OnRenderError() OVERRIDE;

  
  
  void SourceCallback(int fifo_frame_delay, media::AudioBus* audio_bus);

  
  const int source_render_view_id_;
  const int session_id_;

  
  scoped_refptr<media::AudioOutputDevice> sink_;

  
  WebRtcAudioRendererSource* source_;

  
  
  scoped_ptr<int16[]> buffer_;

  
  base::Lock lock_;

  
  int play_ref_count_;

  
  int start_ref_count_;

  
  
  scoped_ptr<media::AudioPullFifo> audio_fifo_;

  
  
  int audio_delay_milliseconds_;

  
  int fifo_delay_milliseconds_;

  
  const int sample_rate_;
  const int frames_per_buffer_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(WebRtcAudioRenderer);
};

}  

#endif  
