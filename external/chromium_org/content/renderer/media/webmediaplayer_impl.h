// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_RENDERER_MEDIA_WEBMEDIAPLAYER_IMPL_H_
#define CONTENT_RENDERER_MEDIA_WEBMEDIAPLAYER_IMPL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "cc/layers/video_frame_provider.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/renderer/media/crypto/proxy_decryptor.h"
#include "media/base/audio_renderer_sink.h"
#include "media/base/decryptor.h"
#include "media/base/media_keys.h"
#include "media/base/pipeline.h"
#include "media/base/text_track.h"
#include "media/filters/skcanvas_video_renderer.h"
#include "skia/ext/platform_canvas.h"
#include "third_party/WebKit/public/platform/WebAudioSourceProvider.h"
#include "third_party/WebKit/public/platform/WebGraphicsContext3D.h"
#include "third_party/WebKit/public/platform/WebMediaPlayer.h"
#include "third_party/WebKit/public/platform/WebMediaPlayerClient.h"
#include "url/gurl.h"

class RenderAudioSourceProvider;

namespace blink {
class WebFrame;
}

namespace base {
class MessageLoopProxy;
}

namespace media {
class ChunkDemuxer;
class GpuVideoAcceleratorFactories;
class MediaLog;
}

namespace webkit {
class WebLayerImpl;
}

namespace content {
class BufferedDataSource;
class WebAudioSourceProviderImpl;
class WebMediaPlayerDelegate;
class WebMediaPlayerParams;
class WebTextTrackImpl;

class WebMediaPlayerImpl
    : public blink::WebMediaPlayer,
      public cc::VideoFrameProvider,
      public content::RenderViewObserver,
      public base::SupportsWeakPtr<WebMediaPlayerImpl> {
 public:
  
  
  
  
  WebMediaPlayerImpl(
      content::RenderView* render_view,
      blink::WebFrame* frame,
      blink::WebMediaPlayerClient* client,
      base::WeakPtr<WebMediaPlayerDelegate> delegate,
      const WebMediaPlayerParams& params);
  virtual ~WebMediaPlayerImpl();

  virtual void load(LoadType load_type,
                    const blink::WebURL& url,
                    CORSMode cors_mode) OVERRIDE;

  
  virtual void play();
  virtual void pause();
  virtual bool supportsFullscreen() const;
  virtual bool supportsSave() const;
  virtual void seek(double seconds);
  virtual void setRate(double rate);
  virtual void setVolume(double volume);
  virtual void setPreload(blink::WebMediaPlayer::Preload preload);
  virtual const blink::WebTimeRanges& buffered();
  virtual double maxTimeSeekable() const;

  
  virtual void paint(blink::WebCanvas* canvas,
                     const blink::WebRect& rect,
                     unsigned char alpha);

  
  virtual bool hasVideo() const;
  virtual bool hasAudio() const;

  
  virtual blink::WebSize naturalSize() const;

  
  virtual bool paused() const;
  virtual bool seeking() const;
  virtual double duration() const;
  virtual double currentTime() const;

  
  
  
  virtual blink::WebMediaPlayer::NetworkState networkState() const;
  virtual blink::WebMediaPlayer::ReadyState readyState() const;

  virtual bool didLoadingProgress() const;

  virtual bool hasSingleSecurityOrigin() const;
  virtual bool didPassCORSAccessCheck() const;

  virtual double mediaTimeForTimeValue(double timeValue) const;

  virtual unsigned decodedFrameCount() const;
  virtual unsigned droppedFrameCount() const;
  virtual unsigned audioDecodedByteCount() const;
  virtual unsigned videoDecodedByteCount() const;

  
  virtual void SetVideoFrameProviderClient(
      cc::VideoFrameProvider::Client* client) OVERRIDE;
  virtual scoped_refptr<media::VideoFrame> GetCurrentFrame() OVERRIDE;
  virtual void PutCurrentFrame(const scoped_refptr<media::VideoFrame>& frame)
      OVERRIDE;

  virtual bool copyVideoTextureToPlatformTexture(
      blink::WebGraphicsContext3D* web_graphics_context,
      unsigned int texture,
      unsigned int level,
      unsigned int internal_format,
      unsigned int type,
      bool premultiply_alpha,
      bool flip_y);

  virtual blink::WebAudioSourceProvider* audioSourceProvider();

  virtual MediaKeyException generateKeyRequest(
      const blink::WebString& key_system,
      const unsigned char* init_data,
      unsigned init_data_length);

  virtual MediaKeyException addKey(const blink::WebString& key_system,
                                   const unsigned char* key,
                                   unsigned key_length,
                                   const unsigned char* init_data,
                                   unsigned init_data_length,
                                   const blink::WebString& session_id);

  virtual MediaKeyException cancelKeyRequest(
      const blink::WebString& key_system,
      const blink::WebString& session_id);

  
  virtual void OnDestruct() OVERRIDE;

  void Repaint();

  void OnPipelineSeek(media::PipelineStatus status);
  void OnPipelineEnded();
  void OnPipelineError(media::PipelineStatus error);
  void OnPipelineBufferingState(
      media::Pipeline::BufferingState buffering_state);
  void OnDemuxerOpened();
  void OnKeyAdded(const std::string& session_id);
  void OnKeyError(const std::string& session_id,
                  media::MediaKeys::KeyError error_code,
                  int system_code);
  void OnKeyMessage(const std::string& session_id,
                    const std::vector<uint8>& message,
                    const std::string& default_url);
  void OnNeedKey(const std::string& type,
                 const std::vector<uint8>& init_data);
  void OnAddTextTrack(const media::TextTrackConfig& config,
                      const media::AddTextTrackDoneCB& done_cb);
  void SetOpaque(bool);

 private:
  
  
  void DoLoad(LoadType load_type,
              const blink::WebURL& url,
              CORSMode cors_mode);

  
  void DataSourceInitialized(const GURL& gurl, bool success);

  
  void NotifyDownloading(bool is_downloading);

  
  void StartPipeline();

  
  
  void SetNetworkState(blink::WebMediaPlayer::NetworkState state);
  void SetReadyState(blink::WebMediaPlayer::ReadyState state);

  
  void Destroy();

  
  blink::WebMediaPlayerClient* GetClient();

  
  void IncrementExternallyAllocatedMemory();

  
  
  MediaKeyException GenerateKeyRequestInternal(
      const blink::WebString& key_system,
      const unsigned char* init_data,
      unsigned init_data_length);
  MediaKeyException AddKeyInternal(const blink::WebString& key_system,
                                   const unsigned char* key,
                                   unsigned key_length,
                                   const unsigned char* init_data,
                                   unsigned init_data_length,
                                   const blink::WebString& session_id);
  MediaKeyException CancelKeyRequestInternal(
      const blink::WebString& key_system,
      const blink::WebString& session_id);

  
  double GetPipelineDuration() const;

  
  void OnDurationChange();

  
  
  void FrameReady(const scoped_refptr<media::VideoFrame>& frame);

  
  
  
  
  void DoneWaitingForPaint(bool painting_frame);

  blink::WebFrame* frame_;

  
  blink::WebMediaPlayer::NetworkState network_state_;
  blink::WebMediaPlayer::ReadyState ready_state_;

  
  blink::WebTimeRanges buffered_;

  
  
  const scoped_refptr<base::MessageLoopProxy> main_loop_;

  scoped_ptr<media::Pipeline> pipeline_;
  scoped_refptr<base::MessageLoopProxy> media_loop_;

  
  
  blink::WebString current_key_system_;

  
  LoadType load_type_;

  
  
  
  
  
  
  
  
  
  
  
  
  bool paused_;
  bool seeking_;
  double playback_rate_;
  base::TimeDelta paused_time_;

  
  
  bool pending_seek_;
  double pending_seek_seconds_;

  blink::WebMediaPlayerClient* client_;

  base::WeakPtr<WebMediaPlayerDelegate> delegate_;

  base::Callback<void(const base::Closure&)> defer_load_cb_;

  scoped_refptr<media::MediaLog> media_log_;

  
  
  bool accelerated_compositing_reported_;

  bool incremented_externally_allocated_memory_;

  
  scoped_refptr<media::GpuVideoAcceleratorFactories> gpu_factories_;

  
  scoped_refptr<WebAudioSourceProviderImpl> audio_source_provider_;

  bool is_local_source_;
  bool supports_save_;

  
  scoped_ptr<ProxyDecryptor> decryptor_;

  bool starting_;

  
  
  
  
  
  
  scoped_ptr<BufferedDataSource> data_source_;
  scoped_ptr<media::Demuxer> demuxer_;
  media::ChunkDemuxer* chunk_demuxer_;

  
  
  std::string init_data_type_;

  
  
  
  
  
  
  mutable base::Lock lock_;
  media::SkCanvasVideoRenderer skcanvas_video_renderer_;
  scoped_refptr<media::VideoFrame> current_frame_;
  bool current_frame_painted_;
  uint32 frames_dropped_before_paint_;
  bool pending_repaint_;
  bool pending_size_change_;

  
  
  scoped_ptr<webkit::WebLayerImpl> video_weblayer_;

  
  
  cc::VideoFrameProvider::Client* video_frame_provider_client_;

  
  int text_track_index_;

  DISALLOW_COPY_AND_ASSIGN(WebMediaPlayerImpl);
};

}  

#endif  
