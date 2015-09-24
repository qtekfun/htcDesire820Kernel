// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "content/common/media/media_player_messages_enums_android.h"
#include "ipc/ipc_message_macros.h"
#include "media/base/android/media_player_android.h"
#include "media/base/android/demuxer_stream_player_params.h"
#include "media/base/media_keys.h"
#include "ui/gfx/rect_f.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START MediaPlayerMsgStart

IPC_ENUM_TRAITS(media::AudioCodec)
IPC_ENUM_TRAITS(media::DemuxerStream::Status)
IPC_ENUM_TRAITS(media::DemuxerStream::Type)
IPC_ENUM_TRAITS(media::MediaKeys::KeyError)
IPC_ENUM_TRAITS(media::VideoCodec)

IPC_STRUCT_TRAITS_BEGIN(media::DemuxerConfigs)
  IPC_STRUCT_TRAITS_MEMBER(audio_codec)
  IPC_STRUCT_TRAITS_MEMBER(audio_channels)
  IPC_STRUCT_TRAITS_MEMBER(audio_sampling_rate)
  IPC_STRUCT_TRAITS_MEMBER(is_audio_encrypted)
  IPC_STRUCT_TRAITS_MEMBER(audio_extra_data)

  IPC_STRUCT_TRAITS_MEMBER(video_codec)
  IPC_STRUCT_TRAITS_MEMBER(video_size)
  IPC_STRUCT_TRAITS_MEMBER(is_video_encrypted)
  IPC_STRUCT_TRAITS_MEMBER(video_extra_data)

  IPC_STRUCT_TRAITS_MEMBER(duration_ms)
#if defined(GOOGLE_TV)
  IPC_STRUCT_TRAITS_MEMBER(key_system)
#endif  
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(media::DemuxerData)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(access_units)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(media::AccessUnit)
  IPC_STRUCT_TRAITS_MEMBER(status)
  IPC_STRUCT_TRAITS_MEMBER(end_of_stream)
  IPC_STRUCT_TRAITS_MEMBER(data)
  IPC_STRUCT_TRAITS_MEMBER(timestamp)
  IPC_STRUCT_TRAITS_MEMBER(key_id)
  IPC_STRUCT_TRAITS_MEMBER(iv)
  IPC_STRUCT_TRAITS_MEMBER(subsamples)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(media::SubsampleEntry)
  IPC_STRUCT_TRAITS_MEMBER(clear_bytes)
  IPC_STRUCT_TRAITS_MEMBER(cypher_bytes)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(MediaPlayerHostMsg_Initialize_Type)




IPC_MESSAGE_ROUTED2(MediaPlayerMsg_MediaBufferingUpdate,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(MediaPlayerMsg_MediaError,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_MediaPlaybackCompleted,
                    int )

IPC_MESSAGE_ROUTED5(MediaPlayerMsg_MediaMetadataChanged,
                    int ,
                    base::TimeDelta ,
                    int ,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED2(MediaPlayerMsg_SeekRequest,
                    int ,
                    base::TimeDelta )

IPC_MESSAGE_ROUTED2(MediaPlayerMsg_SeekCompleted,
                    int ,
                    base::TimeDelta )

IPC_MESSAGE_ROUTED3(MediaPlayerMsg_MediaVideoSizeChanged,
                    int ,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(MediaPlayerMsg_MediaTimeUpdate,
                    int ,
                    base::TimeDelta )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_MediaPlayerReleased,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_DidEnterFullscreen,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_DidExitFullscreen,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_DidMediaPlayerPlay,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_DidMediaPlayerPause,
                    int )

IPC_MESSAGE_CONTROL3(MediaPlayerMsg_DemuxerSeekRequest,
                     int ,
                     base::TimeDelta ,
                     bool )

IPC_MESSAGE_CONTROL2(MediaPlayerMsg_ReadFromDemuxer,
                     int ,
                     media::DemuxerStream::Type )

IPC_MESSAGE_CONTROL1(MediaPlayerMsg_MediaConfigRequest,
                     int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_ConnectedToRemoteDevice,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_DisconnectedFromRemoteDevice,
                    int )

IPC_MESSAGE_ROUTED1(MediaPlayerMsg_RequestFullscreen,
                    int )


IPC_MESSAGE_ROUTED1(MediaPlayerHostMsg_DestroyMediaPlayer,
                    int )

IPC_MESSAGE_ROUTED0(MediaPlayerHostMsg_DestroyAllMediaPlayers)

IPC_MESSAGE_ROUTED5(
    MediaPlayerHostMsg_Initialize,
    MediaPlayerHostMsg_Initialize_Type ,
    int ,
    GURL ,
    GURL ,
    int )

IPC_MESSAGE_ROUTED2(MediaPlayerHostMsg_Pause,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED1(MediaPlayerHostMsg_Release, int )

IPC_MESSAGE_ROUTED2(MediaPlayerHostMsg_Seek,
                    int ,
                    base::TimeDelta )

IPC_MESSAGE_ROUTED1(MediaPlayerHostMsg_Start, int )

IPC_MESSAGE_ROUTED2(MediaPlayerHostMsg_SetVolume,
                    int ,
                    double )

IPC_MESSAGE_ROUTED1(MediaPlayerHostMsg_EnterFullscreen, int )

IPC_MESSAGE_ROUTED1(MediaPlayerHostMsg_ExitFullscreen, int )

IPC_MESSAGE_CONTROL2(MediaPlayerHostMsg_DemuxerSeekDone,
                     int ,
                     base::TimeDelta )

IPC_MESSAGE_CONTROL2(MediaPlayerHostMsg_DemuxerReady,
                     int ,
                     media::DemuxerConfigs)

IPC_MESSAGE_CONTROL2(MediaPlayerHostMsg_ReadFromDemuxerAck,
                     int ,
                     media::DemuxerData)

IPC_MESSAGE_CONTROL2(MediaPlayerHostMsg_DurationChanged,
                     int ,
                     base::TimeDelta )

#if defined(VIDEO_HOLE)
IPC_MESSAGE_ROUTED3(MediaPlayerHostMsg_NotifyExternalSurface,
                    int ,
                    bool ,
                    gfx::RectF )
#endif  


IPC_MESSAGE_ROUTED3(MediaKeysHostMsg_InitializeCDM,
                    int ,
                    std::vector<uint8> ,
                    GURL )

IPC_MESSAGE_ROUTED4(MediaKeysHostMsg_CreateSession,
                    int ,
                    uint32_t ,
                    std::string ,
                    std::vector<uint8> )

IPC_MESSAGE_ROUTED3(MediaKeysHostMsg_UpdateSession,
                    int ,
                    uint32_t ,
                    std::vector<uint8> )

IPC_MESSAGE_ROUTED2(MediaKeysHostMsg_ReleaseSession,
                    int ,
                    uint32_t )

IPC_MESSAGE_ROUTED3(MediaKeysMsg_SessionCreated,
                    int ,
                    uint32_t ,
                    std::string )

IPC_MESSAGE_ROUTED4(MediaKeysMsg_SessionMessage,
                    int ,
                    uint32_t ,
                    std::vector<uint8> ,
                    std::string )

IPC_MESSAGE_ROUTED2(MediaKeysMsg_SessionReady,
                    int ,
                    uint32_t )

IPC_MESSAGE_ROUTED2(MediaKeysMsg_SessionClosed,
                    int ,
                    uint32_t )

IPC_MESSAGE_ROUTED4(MediaKeysMsg_SessionError,
                    int ,
                    uint32_t ,
                    media::MediaKeys::KeyError ,
                    int )
