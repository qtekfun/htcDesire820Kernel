// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_ANDROID_WEBMEDIAPLAYER_ANDROID_H_
#define CONTENT_RENDERER_MEDIA_ANDROID_WEBMEDIAPLAYER_ANDROID_H_

#include <jni.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/time/time.h"
#include "cc/layers/video_frame_provider.h"
#include "content/common/media/media_player_messages_enums_android.h"
#include "content/renderer/media/android/media_info_loader.h"
#include "content/renderer/media/android/media_source_delegate.h"
#include "content/renderer/media/android/stream_texture_factory_android.h"
#include "content/renderer/media/crypto/proxy_decryptor.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "media/base/android/media_player_android.h"
#include "media/base/demuxer_stream.h"
#include "media/base/media_keys.h"
#include "third_party/WebKit/public/platform/WebGraphicsContext3D.h"
#include "third_party/WebKit/public/platform/WebMediaPlayer.h"
#include "third_party/WebKit/public/platform/WebSize.h"
#include "third_party/WebKit/public/platform/WebURL.h"
#include "ui/gfx/rect_f.h"

namespace media {
class Demuxer;
class MediaLog;
}

namespace blink {
class WebFrame;
}

namespace webkit {
class WebLayerImpl;
}

namespace content {
class WebMediaPlayerDelegate;
class RendererMediaPlayerManager;

#if defined(GOOGLE_TV)
class MediaStreamAudioRenderer;
class MediaStreamClient;
#endif

class WebMediaPlayerAndroid
    : public blink::WebMediaPlayer,
      public cc::VideoFrameProvider,
      public base::MessageLoop::DestructionObserver,
      public base::SupportsWeakPtr<WebMediaPlayerAndroid> {
 public:
  
  
  
  
  
  
  
  WebMediaPlayerAndroid(
      blink::WebFrame* frame,
      blink::WebMediaPlayerClient* client,
      base::WeakPtr<WebMediaPlayerDelegate> delegate,
      RendererMediaPlayerManager* manager,
      StreamTextureFactory* factory,
      const scoped_refptr<base::MessageLoopProxy>& media_loop,
      media::MediaLog* media_log);
  virtual ~WebMediaPlayerAndroid();

  
  virtual void enterFullscreen();
  virtual void exitFullscreen();
  virtual bool canEnterFullscreen() const;

  
  virtual void load(LoadType load_type,
                    const blink::WebURL& url,
                    CORSMode cors_mode) OVERRIDE;

  
  virtual void play();
  virtual void pause();
  virtual void pause(bool is_media_related_action);
  virtual void seek(double seconds);
  virtual bool supportsFullscreen() const;
  virtual bool supportsSave() const;
  virtual void setRate(double rate);
  virtual void setVolume(double volume);
  virtual const blink::WebTimeRanges& buffered();
  virtual double maxTimeSeekable() const;

  
  virtual void paint(blink::WebCanvas* canvas,
                     const blink::WebRect& rect,
                     unsigned char alpha);

  virtual bool copyVideoTextureToPlatformTexture(
      blink::WebGraphicsContext3D* web_graphics_context,
      unsigned int texture,
      unsigned int level,
      unsigned int internal_format,
      unsigned int type,
      bool premultiply_alpha,
      bool flip_y);

  
  virtual bool hasVideo() const;
  virtual bool hasAudio() const;

  
  virtual blink::WebSize naturalSize() const;

  
  virtual bool paused() const;
  virtual bool seeking() const;
  virtual double duration() const;
  virtual double currentTime() const;

  virtual bool didLoadingProgress() const;

  
  virtual blink::WebMediaPlayer::NetworkState networkState() const;
  virtual blink::WebMediaPlayer::ReadyState readyState() const;

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

  
  void OnMediaMetadataChanged(const base::TimeDelta& duration, int width,
                              int height, bool success);
  void OnPlaybackComplete();
  void OnBufferingUpdate(int percentage);
  void OnSeekRequest(const base::TimeDelta& time_to_seek);
  void OnSeekComplete(const base::TimeDelta& current_time);
  void OnMediaError(int error_type);
  void OnVideoSizeChanged(int width, int height);
  void OnDurationChanged(const base::TimeDelta& duration);

  
  void OnTimeUpdate(const base::TimeDelta& current_time);

  
  void OnConnectedToRemoteDevice();
  void OnDisconnectedFromRemoteDevice();
  void OnDidEnterFullscreen();
  void OnDidExitFullscreen();
  void OnMediaPlayerPlay();
  void OnMediaPlayerPause();
  void OnRequestFullscreen();

  
  virtual void OnPlayerReleased();

  
  
  
  virtual void ReleaseMediaResources();

  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  
  void Detach();

#if defined(VIDEO_HOLE)
  
  
  
  bool RetrieveGeometryChange(gfx::RectF* rect);
#endif  

  virtual MediaKeyException generateKeyRequest(
      const blink::WebString& key_system,
      const unsigned char* init_data,
      unsigned init_data_length) OVERRIDE;
  virtual MediaKeyException addKey(
      const blink::WebString& key_system,
      const unsigned char* key,
      unsigned key_length,
      const unsigned char* init_data,
      unsigned init_data_length,
      const blink::WebString& session_id) OVERRIDE;
  virtual MediaKeyException cancelKeyRequest(
      const blink::WebString& key_system,
      const blink::WebString& session_id) OVERRIDE;

  void OnKeyAdded(const std::string& session_id);
  void OnKeyError(const std::string& session_id,
                  media::MediaKeys::KeyError error_code,
                  int system_code);
  void OnKeyMessage(const std::string& session_id,
                    const std::vector<uint8>& message,
                    const std::string& destination_url);

  void OnMediaSourceOpened(blink::WebMediaSource* web_media_source);

  void OnNeedKey(const std::string& type,
                 const std::vector<uint8>& init_data);

#if defined(GOOGLE_TV)
  bool InjectMediaStream(MediaStreamClient* media_stream_client,
                         media::Demuxer* demuxer,
                         const base::Closure& destroy_demuxer_cb);
#endif

  
  static void OnReleaseRemotePlaybackTexture(
      const scoped_refptr<base::MessageLoopProxy>& main_loop,
      const base::WeakPtr<WebMediaPlayerAndroid>& player,
      uint32 sync_point);

 protected:
  
  void UpdatePlayingState(bool is_playing_);

  
  void UpdateNetworkState(blink::WebMediaPlayer::NetworkState state);
  void UpdateReadyState(blink::WebMediaPlayer::ReadyState state);
  void TryCreateStreamTextureProxyIfNeeded();
  void DoCreateStreamTexture();


  
  
  void EstablishSurfaceTexturePeer();

  
  void SetNeedsEstablishPeer(bool needs_establish_peer);

 private:
  void DrawRemotePlaybackIcon();
  void ReallocateVideoFrame();
  void SetCurrentFrameInternal(scoped_refptr<media::VideoFrame>& frame);
  void DidLoadMediaInfo(MediaInfoLoader::Status status);
  void DoReleaseRemotePlaybackTexture(uint32 sync_point);

  bool IsKeySystemSupported(const blink::WebString& key_system);

  
  
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

  blink::WebFrame* const frame_;

  blink::WebMediaPlayerClient* const client_;

  
  
  
  
  
  base::WeakPtr<WebMediaPlayerDelegate> delegate_;

  
  blink::WebTimeRanges buffered_;

  
  blink::WebSize natural_size_;

  
  blink::WebSize cached_stream_texture_size_;

  
  scoped_refptr<media::VideoFrame> current_frame_;
  base::Lock current_frame_lock_;

  base::ThreadChecker main_thread_checker_;

  
  const scoped_refptr<base::MessageLoopProxy> main_loop_;

  
  const scoped_refptr<base::MessageLoopProxy> media_loop_;

  
  GURL url_;

  
  base::TimeDelta duration_;

  
  
  
  bool ignore_metadata_duration_change_;

  
  
  bool pending_seek_;
  base::TimeDelta pending_seek_time_;

  
  bool seeking_;
  base::TimeDelta seek_time_;

  
  mutable bool did_loading_progress_;

  
  
  RendererMediaPlayerManager* manager_;

  
  int player_id_;

  
  blink::WebMediaPlayer::NetworkState network_state_;
  blink::WebMediaPlayer::ReadyState ready_state_;

  
  unsigned int remote_playback_texture_id_;

  
  unsigned int texture_id_;

  
  
  gpu::Mailbox texture_mailbox_;
  unsigned int texture_mailbox_sync_point_;

  
  unsigned int stream_id_;

  
  bool is_playing_;

  
  bool playing_started_;

  
  bool needs_establish_peer_;

  
  bool stream_texture_proxy_initialized_;

  
  bool has_size_info_;

  
  bool has_media_metadata_;
  bool has_media_info_;

  
  scoped_ptr<StreamTextureFactory> stream_texture_factory_;

  
  
  ScopedStreamTextureProxy stream_texture_proxy_;

  
  bool needs_external_surface_;

  
  
  cc::VideoFrameProvider::Client* video_frame_provider_client_;

  scoped_ptr<webkit::WebLayerImpl> video_weblayer_;

#if defined(VIDEO_HOLE)
  
  
  gfx::RectF last_computed_rect_;
#endif  

#if defined(GOOGLE_TV)
  
  
  int external_surface_threshold_;

  
  media::Demuxer* demuxer_;
  base::Closure destroy_demuxer_cb_;
  scoped_refptr<MediaStreamAudioRenderer> audio_renderer_;
  MediaStreamClient* media_stream_client_;
#endif

  scoped_ptr<MediaSourceDelegate,
             MediaSourceDelegate::Destroyer> media_source_delegate_;

  
  
  bool pending_playback_;

  MediaPlayerHostMsg_Initialize_Type player_type_;

  
  
  
  double current_time_;

  
  bool is_remote_;

  media::MediaLog* media_log_;

  scoped_ptr<MediaInfoLoader> info_loader_;

  
  
  blink::WebString current_key_system_;

  
  
  std::string init_data_type_;

  
  scoped_ptr<ProxyDecryptor> decryptor_;

  base::WeakPtrFactory<WebMediaPlayerAndroid> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebMediaPlayerAndroid);
};

}  

#endif  
