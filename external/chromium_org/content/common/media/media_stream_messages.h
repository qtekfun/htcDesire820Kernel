// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"
#include "ipc/ipc_message_macros.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START MediaStreamMsgStart

IPC_ENUM_TRAITS_MAX_VALUE(content::MediaStreamType,
                          content::NUM_MEDIA_TYPES - 1)

IPC_ENUM_TRAITS_MAX_VALUE(content::VideoFacingMode,
                          content::NUM_MEDIA_VIDEO_FACING_MODE - 1)

IPC_STRUCT_TRAITS_BEGIN(content::StreamOptions::Constraint)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(value)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::StreamOptions)
  IPC_STRUCT_TRAITS_MEMBER(audio_requested)
  IPC_STRUCT_TRAITS_MEMBER(mandatory_audio)
  IPC_STRUCT_TRAITS_MEMBER(optional_audio)
  IPC_STRUCT_TRAITS_MEMBER(video_requested)
  IPC_STRUCT_TRAITS_MEMBER(mandatory_video)
  IPC_STRUCT_TRAITS_MEMBER(optional_video)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::StreamDeviceInfo)
  IPC_STRUCT_TRAITS_MEMBER(device.type)
  IPC_STRUCT_TRAITS_MEMBER(device.name)
  IPC_STRUCT_TRAITS_MEMBER(device.id)
  IPC_STRUCT_TRAITS_MEMBER(device.video_facing)
  IPC_STRUCT_TRAITS_MEMBER(device.matched_output_device_id)
  IPC_STRUCT_TRAITS_MEMBER(device.input.sample_rate)
  IPC_STRUCT_TRAITS_MEMBER(device.input.channel_layout)
  IPC_STRUCT_TRAITS_MEMBER(device.input.frames_per_buffer)
  IPC_STRUCT_TRAITS_MEMBER(device.input.effects)
  IPC_STRUCT_TRAITS_MEMBER(device.matched_output.sample_rate)
  IPC_STRUCT_TRAITS_MEMBER(device.matched_output.channel_layout)
  IPC_STRUCT_TRAITS_MEMBER(device.matched_output.frames_per_buffer)
  IPC_STRUCT_TRAITS_MEMBER(session_id)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_ROUTED4(MediaStreamMsg_StreamGenerated,
                    int ,
                    std::string ,
                    content::StreamDeviceInfoArray ,
                    content::StreamDeviceInfoArray )

IPC_MESSAGE_ROUTED1(MediaStreamMsg_StreamGenerationFailed,
                    int )

IPC_MESSAGE_ROUTED2(MediaStreamMsg_DeviceStopped,
                    std::string ,
                    content::StreamDeviceInfo )

IPC_MESSAGE_ROUTED2(MediaStreamMsg_DevicesEnumerated,
                    int ,
                    content::StreamDeviceInfoArray )

IPC_MESSAGE_ROUTED3(MediaStreamMsg_DeviceOpened,
                    int ,
                    std::string ,
                    content::StreamDeviceInfo )

IPC_MESSAGE_ROUTED1(MediaStreamMsg_DeviceOpenFailed,
                    int )

IPC_MESSAGE_CONTROL2(MediaStreamMsg_GetSourcesACK,
                     int ,
                     content::StreamDeviceInfoArray )


IPC_MESSAGE_CONTROL4(MediaStreamHostMsg_GenerateStream,
                     int ,
                     int ,
                     content::StreamOptions ,
                     GURL )

IPC_MESSAGE_CONTROL2(MediaStreamHostMsg_CancelGenerateStream,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(MediaStreamHostMsg_StopStreamDevice,
                     int ,
                     std::string )

IPC_MESSAGE_CONTROL2(MediaStreamHostMsg_GetSources,
                     int ,
                     GURL )


IPC_MESSAGE_CONTROL4(MediaStreamHostMsg_EnumerateDevices,
                     int ,
                     int ,
                     content::MediaStreamType ,
                     GURL )

IPC_MESSAGE_CONTROL2(MediaStreamHostMsg_CancelEnumerateDevices,
                     int ,
                     int )

IPC_MESSAGE_CONTROL5(MediaStreamHostMsg_OpenDevice,
                     int ,
                     int ,
                     std::string ,
                     content::MediaStreamType ,
                     GURL )

IPC_MESSAGE_CONTROL2(MediaStreamHostMsg_CloseDevice,
                     int ,
                     std::string )
