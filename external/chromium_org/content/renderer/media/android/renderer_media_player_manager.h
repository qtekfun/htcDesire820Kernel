// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_ANDROID_RENDERER_MEDIA_PLAYER_MANAGER_H_
#define CONTENT_RENDERER_MEDIA_ANDROID_RENDERER_MEDIA_PLAYER_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "content/common/media/media_player_messages_enums_android.h"
#include "content/public/renderer/render_view_observer.h"
#include "media/base/android/media_player_android.h"
#include "media/base/media_keys.h"
#include "url/gurl.h"

#if defined(GOOGLE_TV)
#include "ui/gfx/rect_f.h"
#endif

namespace blink {
class WebFrame;
}

namespace gfx {
class RectF;
}

namespace content {

class ProxyMediaKeys;
class WebMediaPlayerAndroid;

class RendererMediaPlayerManager : public RenderViewObserver {
 public:
  
  RendererMediaPlayerManager(RenderView* render_view);
  virtual ~RendererMediaPlayerManager();

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  void Initialize(MediaPlayerHostMsg_Initialize_Type type,
                  int player_id,
                  const GURL& url,
                  const GURL& first_party_for_cookies,
                  int demuxer_client_id);

  
  void Start(int player_id);

  
  
  
  
  
  void Pause(int player_id, bool is_media_related_action);

  
  void Seek(int player_id, const base::TimeDelta& time);

  
  void SetVolume(int player_id, double volume);

  
  void ReleaseResources(int player_id);

  
  void DestroyPlayer(int player_id);

  
  void EnterFullscreen(int player_id, blink::WebFrame* frame);

  
  void ExitFullscreen(int player_id);

#if defined(VIDEO_HOLE)
  
  void RequestExternalSurface(int player_id, const gfx::RectF& geometry);

  
  virtual void DidCommitCompositorFrame() OVERRIDE;
#endif  

  
  void InitializeCDM(int media_keys_id,
                     ProxyMediaKeys* media_keys,
                     const std::vector<uint8>& uuid,
                     const GURL& frame_url);
  void CreateSession(int media_keys_id,
                     uint32 session_id,
                     const std::string& type,
                     const std::vector<uint8>& init_data);
  void UpdateSession(int media_keys_id,
                     uint32 session_id,
                     const std::vector<uint8>& response);
  void ReleaseSession(int media_keys_id, uint32 session_id);

  
  int RegisterMediaPlayer(WebMediaPlayerAndroid* player);
  void UnregisterMediaPlayer(int player_id);

  
  
  
  
  void RegisterMediaKeys(int media_keys_id, ProxyMediaKeys* media_keys);

  
  
  void ReleaseVideoResources();

  
  bool CanEnterFullscreen(blink::WebFrame* frame);

  
  void DidEnterFullscreen(blink::WebFrame* frame);
  void DidExitFullscreen();

  
  bool IsInFullscreen(blink::WebFrame* frame);

  
  bool ShouldEnterFullscreen(blink::WebFrame* frame);

  
  WebMediaPlayerAndroid* GetMediaPlayer(int player_id);

  
  ProxyMediaKeys* GetMediaKeys(int media_keys_id);

#if defined(VIDEO_HOLE)
  
  void RetrieveGeometryChanges(std::map<int, gfx::RectF>* changes);
#endif  

 private:
  
  void OnMediaMetadataChanged(int player_id,
                              base::TimeDelta duration,
                              int width,
                              int height,
                              bool success);
  void OnMediaPlaybackCompleted(int player_id);
  void OnMediaBufferingUpdate(int player_id, int percent);
  void OnSeekRequest(int player_id, const base::TimeDelta& time_to_seek);
  void OnSeekCompleted(int player_id, const base::TimeDelta& current_time);
  void OnMediaError(int player_id, int error);
  void OnVideoSizeChanged(int player_id, int width, int height);
  void OnTimeUpdate(int player_id, base::TimeDelta current_time);
  void OnMediaPlayerReleased(int player_id);
  void OnConnectedToRemoteDevice(int player_id);
  void OnDisconnectedFromRemoteDevice(int player_id);
  void OnDidExitFullscreen(int player_id);
  void OnDidEnterFullscreen(int player_id);
  void OnPlayerPlay(int player_id);
  void OnPlayerPause(int player_id);
  void OnRequestFullscreen(int player_id);
  void OnSessionCreated(int media_keys_id,
                        uint32 session_id,
                        const std::string& web_session_id);
  void OnSessionMessage(int media_keys_id,
                        uint32 session_id,
                        const std::vector<uint8>& message,
                        const std::string& destination_url);
  void OnSessionReady(int media_keys_id, uint32 session_id);
  void OnSessionClosed(int media_keys_id, uint32 session_id);
  void OnSessionError(int media_keys_id,
                      uint32 session_id,
                      media::MediaKeys::KeyError error_code,
                      int system_code);

  
  
  std::map<int, WebMediaPlayerAndroid*> media_players_;

  
  
  std::map<int, ProxyMediaKeys*> media_keys_;

  int next_media_player_id_;

  
  blink::WebFrame* fullscreen_frame_;

  
  blink::WebFrame* pending_fullscreen_frame_;

  DISALLOW_COPY_AND_ASSIGN(RendererMediaPlayerManager);
};

}  

#endif  
