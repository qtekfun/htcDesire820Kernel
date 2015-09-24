// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_MEDIA_PLAYER_MANAGER_H_
#define MEDIA_BASE_ANDROID_MEDIA_PLAYER_MANAGER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "media/base/android/demuxer_stream_player_params.h"
#include "media/base/media_export.h"
#include "media/base/media_keys.h"

namespace media {

class MediaDrmBridge;
class MediaPlayerAndroid;
class MediaResourceGetter;

class MEDIA_EXPORT MediaPlayerManager {
 public:
  virtual ~MediaPlayerManager() {}

  
  
  
  
  virtual void RequestMediaResources(int player_id) = 0;

  
  
  virtual void ReleaseMediaResources(int player_id) = 0;

  
  virtual MediaResourceGetter* GetMediaResourceGetter() = 0;

  
  
  virtual void OnTimeUpdate(int player_id, base::TimeDelta current_time) = 0;

  
  
  virtual void OnMediaMetadataChanged(
      int player_id,
      base::TimeDelta duration,
      int width,
      int height,
      bool success) = 0;

  
  virtual void OnPlaybackComplete(int player_id) = 0;

  
  virtual void OnMediaInterrupted(int player_id) = 0;

  
  
  virtual void OnBufferingUpdate(int player_id, int percentage) = 0;

  
  virtual void OnSeekComplete(
      int player_id,
      const base::TimeDelta& current_time) = 0;

  
  virtual void OnError(int player_id, int error) = 0;

  
  virtual void OnVideoSizeChanged(int player_id, int width, int height) = 0;

  
  virtual MediaPlayerAndroid* GetFullscreenPlayer() = 0;

  
  virtual MediaPlayerAndroid* GetPlayer(int player_id) = 0;

  
  virtual void DestroyAllMediaPlayers() = 0;

  
  virtual media::MediaDrmBridge* GetDrmBridge(int media_keys_id) = 0;

  
  virtual void OnProtectedSurfaceRequested(int player_id) = 0;

  
  
  
  

  
  virtual void OnSessionCreated(int media_keys_id,
                                uint32 session_id,
                                const std::string& web_session_id) = 0;

  
  virtual void OnSessionMessage(int media_keys_id,
                                uint32 session_id,
                                const std::vector<uint8>& message,
                                const std::string& destination_url) = 0;

  
  virtual void OnSessionReady(int media_keys_id, uint32 session_id) = 0;

  
  virtual void OnSessionClosed(int media_keys_id, uint32 session_id) = 0;

  
  virtual void OnSessionError(int media_keys_id,
                              uint32 session_id,
                              media::MediaKeys::KeyError error_code,
                              int system_code) = 0;
};

}  

#endif  
