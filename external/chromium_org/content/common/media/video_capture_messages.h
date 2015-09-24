// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/memory/shared_memory.h"
#include "content/common/content_export.h"
#include "content/common/media/video_capture.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "media/video/capture/video_capture_types.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START VideoCaptureMsgStart

IPC_ENUM_TRAITS(content::VideoCaptureState)

IPC_STRUCT_TRAITS_BEGIN(media::VideoCaptureParams)
  IPC_STRUCT_TRAITS_MEMBER(requested_format)
  IPC_STRUCT_TRAITS_MEMBER(allow_resolution_change)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_CONTROL2(VideoCaptureMsg_StateChanged,
                     int ,
                     content::VideoCaptureState )

IPC_MESSAGE_CONTROL4(VideoCaptureMsg_NewBuffer,
                     int ,
                     base::SharedMemoryHandle ,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(VideoCaptureMsg_FreeBuffer,
                     int ,
                     int )

IPC_MESSAGE_CONTROL4(VideoCaptureMsg_BufferReady,
                     int ,
                     int ,
                     base::Time ,
                     media::VideoCaptureFormat )

IPC_MESSAGE_CONTROL3(VideoCaptureHostMsg_Start,
                     int ,
                     media::VideoCaptureSessionId, 
                     media::VideoCaptureParams )

IPC_MESSAGE_CONTROL1(VideoCaptureHostMsg_Pause,
                     int )

IPC_MESSAGE_CONTROL1(VideoCaptureHostMsg_Stop,
                     int )

IPC_MESSAGE_CONTROL2(VideoCaptureHostMsg_BufferReady,
                     int ,
                     int )
