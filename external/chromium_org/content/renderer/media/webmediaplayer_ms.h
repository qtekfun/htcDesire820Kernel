// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBMEDIAPLAYER_MS_H_
#define CONTENT_RENDERER_MEDIA_WEBMEDIAPLAYER_MS_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "cc/layers/video_frame_provider.h"
#include "media/filters/skcanvas_video_renderer.h"
#include "skia/ext/platform_canvas.h"
#include "third_party/WebKit/public/platform/WebMediaPlayer.h"
#include "url/gurl.h"

namespace blink {
class WebFrame;
class WebMediaPlayerClient;
}

namespace media {
class MediaLog;
}

namespace webkit {
class WebLayerImpl;
}

namespace content {
class MediaStreamAudioRenderer;
class MediaStreamClient;
class VideoFrameProvider;
class WebMediaPlayerDelegate;

class WebMediaPlayerMS
    : public blink::WebMediaPlayer,
      public cc::VideoFrameProvider,
      public base::SupportsWeakPtr<WebMediaPlayerMS> {
 public:
  
  
  WebMediaPlayerMS(blink::WebFrame* frame,
                   blink::WebMediaPlayerClient* client,
                   base::WeakPtr<WebMediaPlayerDelegate> delegate,
                   MediaStreamClient* media_stream_client,
                   media::MediaLog* media_log);
  virtual ~WebMediaPlayerMS();

  virtual void load(LoadType load_type,
                    const blink::WebURL& url,
                    CORSMode cors_mode) OVERRIDE;

  
  virtual void play() OVERRIDE;
  virtual void pause() OVERRIDE;
  virtual bool supportsFullscreen() const OVERRIDE;
  virtual bool supportsSave() const OVERRIDE;
  virtual void seek(double seconds);
  virtual void setRate(double rate);
  virtual void setVolume(double volume);
  virtual void setPreload(blink::WebMediaPlayer::Preload preload) OVERRIDE;
  virtual const blink::WebTimeRanges& buffered() OVERRIDE;
  virtual double maxTimeSeekable() const;

  
  virtual void paint(blink::WebCanvas* canvas,
                     const blink::WebRect& rect,
                     unsigned char alpha) OVERRIDE;

  
  virtual bool hasVideo() const OVERRIDE;
  virtual bool hasAudio() const OVERRIDE;

  
  virtual blink::WebSize naturalSize() const OVERRIDE;

  
  virtual bool paused() const OVERRIDE;
  virtual bool seeking() const OVERRIDE;
  virtual double duration() const;
  virtual double currentTime() const;

  
  virtual blink::WebMediaPlayer::NetworkState networkState() const OVERRIDE;
  virtual blink::WebMediaPlayer::ReadyState readyState() const OVERRIDE;

  virtual bool didLoadingProgress() const OVERRIDE;

  virtual bool hasSingleSecurityOrigin() const OVERRIDE;
  virtual bool didPassCORSAccessCheck() const OVERRIDE;

  virtual double mediaTimeForTimeValue(double timeValue) const;

  virtual unsigned decodedFrameCount() const OVERRIDE;
  virtual unsigned droppedFrameCount() const OVERRIDE;
  virtual unsigned audioDecodedByteCount() const OVERRIDE;
  virtual unsigned videoDecodedByteCount() const OVERRIDE;

  
  virtual void SetVideoFrameProviderClient(
      cc::VideoFrameProvider::Client* client) OVERRIDE;
  virtual scoped_refptr<media::VideoFrame> GetCurrentFrame() OVERRIDE;
  virtual void PutCurrentFrame(const scoped_refptr<media::VideoFrame>& frame)
      OVERRIDE;

 private:
  
  void OnFrameAvailable(const scoped_refptr<media::VideoFrame>& frame);
  
  void RepaintInternal();

  
  void OnSourceError();

  
  
  void SetNetworkState(blink::WebMediaPlayer::NetworkState state);
  void SetReadyState(blink::WebMediaPlayer::ReadyState state);

  
  blink::WebMediaPlayerClient* GetClient();

  blink::WebFrame* frame_;

  blink::WebMediaPlayer::NetworkState network_state_;
  blink::WebMediaPlayer::ReadyState ready_state_;

  blink::WebTimeRanges buffered_;

  
  base::ThreadChecker thread_checker_;

  blink::WebMediaPlayerClient* client_;

  base::WeakPtr<WebMediaPlayerDelegate> delegate_;

  MediaStreamClient* media_stream_client_;

  
  scoped_refptr<content::VideoFrameProvider> video_frame_provider_;
  bool paused_;

  
  
  
  scoped_refptr<media::VideoFrame> current_frame_;
  
  
  
  bool current_frame_used_;
  base::Lock current_frame_lock_;
  bool pending_repaint_;

  scoped_ptr<webkit::WebLayerImpl> video_weblayer_;

  
  
  cc::VideoFrameProvider::Client* video_frame_provider_client_;

  bool received_first_frame_;
  bool sequence_started_;
  base::TimeDelta start_time_;
  unsigned total_frame_count_;
  unsigned dropped_frame_count_;
  media::SkCanvasVideoRenderer video_renderer_;

  scoped_refptr<MediaStreamAudioRenderer> audio_renderer_;

  scoped_refptr<media::MediaLog> media_log_;

  DISALLOW_COPY_AND_ASSIGN(WebMediaPlayerMS);
};

}  

#endif  
